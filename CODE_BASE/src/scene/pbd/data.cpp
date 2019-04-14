#include "data.h"

#ifdef PBD

/*************************************/
/*************************************/
/************ PARTICLES***************/
/*************************************/
/*************************************/

    simulationData::Particles::Particles() :
            numParticles(0),
            positions(DM(0, 0)),
            velocities(DM(0, 0)),
            inverseMasses(DM()) {
    }

    simulationData::Particles::~Particles() {
        positions.resize(0, 0);
        velocities.resize(0, 0);
        inverseMasses.resize(0, 0);
    }

    void simulationData::Particles::setUpSizes(const int &numParticles) {
        // check for resizing
        this->numParticles = numParticles;
        inverseMasses.resize(numParticles, numParticles);
        inverseMasses.setZero(numParticles, numParticles);
        inverseMasses.setIdentity();
        positions.setZero(numParticles, dim);
        velocities.setZero(numParticles, dim);
    }

    void simulationData::Particles::resizeVelocityAndMassesFromPositions() {
        // assuming positions and numParticles is already set up properly
        velocities.setZero(numParticles, dim);
        inverseMasses.setZero(numParticles, numParticles);
    }

    void simulationData::Particles::addPositions(vector<V>& newPositions) {
        positions.resize(numParticles + newPositions.size(), dim);

        for (int i = 0; i < (int)newPositions.size(); ++i) {
            positions(i + numParticles, 0) = newPositions[i][0];
            positions(i + numParticles, 1) = newPositions[i][1];
            positions(i + numParticles, 2) = newPositions[i][2];
        }
        numParticles += static_cast<int>(newPositions.size());
    }

    DM& simulationData::Particles::getPositions() {
        return positions;
    }

    DM& simulationData::Particles::getVelocities() {
        return velocities;
    }

    DM& simulationData::Particles::getInverseMasses() {
        return inverseMasses;
    }

    void simulationData::Particles::updateValues(DM& x, DM& v, DM& w) {
        positions = x;
        velocities = v;
        inverseMasses = w;
    }



    simulationData::SegmentMesh::SegmentMesh() :
            simParticles(Particles()),
            indices(vector<vector<int>>()) {}

    simulationData::SegmentMesh::~SegmentMesh() {
        indices.clear();
        simParticles.~Particles();
    }

    void simulationData::SegmentMesh::setUpSizes(const int numParticles) {
        simParticles.setUpSizes(numParticles);
    }

    void simulationData::SegmentMesh::addSegment(int indexA, int indexB) {
        indices[indexA].push_back(indexB);
        indices[indexB].push_back(indexA);
    }

    void simulationData::SegmentMesh::addSegments(int startingA, vector<int>& attachedBs) {
        if (testing && startingA > (int)indices.size()) { throw; }

        if (startingA == (int)indices.size()) {
            indices.push_back(vector<int>());
        }

        for (int i = 0; i < (int)attachedBs.size(); ++i) {
            indices[startingA].push_back(attachedBs[i]);
        }
    }

/*************************************/
/*************************************/
/*************** FACE ****************/
/*************************************/
/*************************************/

    simulationData::Face::Face(vector<int> indicesInput, int inputId)
            : indices(indicesInput), attachedFaces(vector<Face*>()), id(inputId) {}
    simulationData::Face::~Face() { indices.clear(); attachedFaces.clear(); }

    bool simulationData::Face::containsIndex(int i) {
        return (indices[0] == i || indices[1] == i || indices[2] == i);
    }

    bool simulationData::Face::containsIndices(int i, int j) {
        return containsIndex(i) && containsIndex(j);
    }

    bool simulationData::Face::containsOrderedIndices(int i, int j) {
        return ((indices[0] == i && indices[1] == j)
                || (indices[1] == i && indices[2] == j)
                || (indices[2] == i && indices[0] == j));
    }

    bool simulationData::Face::adjacentToFaceById(int faceId) {
        // returns true if my attached faces vector already contains this face
        for (int i = 0; i < (int) attachedFaces.size(); ++i) {
            if (attachedFaces[i]->id == faceId) {
                return true;
            }
        }
        return false;
    }

    int simulationData::Face::getAdjacentFaceIndexByIndices(int sharedI, int sharedJ) {
        for (int i = 0; i < (int) attachedFaces.size(); ++i) {
            if (attachedFaces[i]->containsIndices(sharedI, sharedJ)) {
                return i;
            }
        }
        if (testing) { throw; }
        return -1;
    }

    bool simulationData::Face::adjacentToFace(Face* adjacentFace) {
        return adjacentToFaceById(adjacentFace->id);
    }

    bool simulationData::Face::shouldBeAdjacentToFace(Face* face) {
        // if not already adjacent and contains a shared edge with this face
        // adds face to vector of attachedFaces vector and returns true; false otherwise
        if (!adjacentToFace(face)) {
            if (containsIndices(face->indices[0], face->indices[1])
                    || containsIndices(face->indices[1], face->indices[2])
                    || containsIndices(face->indices[2], face->indices[0])) {
                attachedFaces.push_back(face);
                face->shouldBeAdjacentToFace(this);

                return true;
            }
        }
        return false;
    }

    bool simulationData::Face::removeAdjacentFaceById(int id) {
        if (!adjacentToFaceById(id)) {
            return false;
        }

        bool ret = false;
        vector<Face*> attachedNew = vector<Face*>();
        for (int i = 0; i < (int) attachedFaces.size() && !ret; ++i) {
            if (attachedFaces[i]->id != id) {
                attachedNew.push_back(attachedFaces[i]);
                ret = true;
            }
        }
        attachedFaces = attachedNew;
        return ret;
    }

    simulationData::Face* simulationData::Face::getAdjacentById(int faceId) {
        if (testing && !(adjacentToFaceById(faceId))) { throw; }

        for (int i = 0; i < (int) attachedFaces.size(); ++i) {
            if (attachedFaces[i]->id == faceId) {
                return (attachedFaces[i]);
            }
        }
        if (testing) { throw; }
        return nullptr;
    }

    int simulationData::Face::getNonListedIndex(int pi_index, int pj_index) {
        // returns index of point making face's triangle that is not one of the listed inputs
        for (int i = 0; i < (int) indices.size(); ++i) {
            if (indices[i] != pi_index && indices[i] != pj_index) {
                return indices[i];
            }
        }
        if (testing) { throw; }
        return -1;
    }

    void simulationData::Face::resetIndices(int index0, int index1, int index2) {
        indices[0] = index0;
        indices[1] = index1;
        indices[2] = index2;
    }

/*************************************/
/*************************************/
/*********** TRIANGLE MESH ***********/
/*************************************/
/*************************************/

    simulationData::TriangleMesh::TriangleMesh() :
            simParticles(Particles()),
            faces(vector<Face*>()){}

    simulationData::TriangleMesh::~TriangleMesh() {
        faces.clear();
        simParticles.~Particles();
    }

    void simulationData::TriangleMesh::setUpSizes(const int& numParticles) {
        int input = (numParticles == -1) ? simParticles.numParticles : numParticles;
        simParticles.setUpSizes(input);
    }

    void simulationData::TriangleMesh::calculateWeights(
        std::vector<int>& static_particles,
        Particles* simParticles) {
        // weight of particles set to 1 unless static position - if static, weight is 0
        // (maintains static position and prevents other constraints from adjusting them)

        simParticles->inverseMasses.setIdentity(simParticles->numParticles, simParticles->numParticles);
        for (int i = 0; i < (int)static_particles.size(); ++i) {
            simParticles->inverseMasses(static_particles[i], static_particles[i]) = 0;
        }
    }

    void simulationData::TriangleMesh::calculateAdjacentFaces(const int& num_faces) {
        DM check = DM::Zero(num_faces, num_faces);
        for (int i = 0; i < num_faces; ++i) {
            for (int j = 0; j < num_faces; ++j) {
                if (i != j && check(i, j) != 1) {

                    check(i, j) = 1;
                    check(j, i) = 1;

                    if ( faces[i]->shouldBeAdjacentToFace(faces[j]) ) {
                        faces[j]->shouldBeAdjacentToFace(faces[i]);
                    }
                }
            }
        }
    }

#endif