#ifdef PBD

#pragma once

#include "eigen_base.h"

namespace simulationData {

    class Particles {
    public:
        int numParticles;
        DM positions;
        DM velocities;
        DM inverseMasses;

        Particles();
        ~Particles();

        void setUpSizes(const int &numParticles);

        void resizeVelocityAndMassesFromPositions();

        void addPositions(vector<V>& newPositions);

        DM& getPositions();
        DM& getVelocities();
        DM& getInverseMasses();

        void updateValues(DM& x, DM& v, DM& w);
    };

    class SegmentMesh {
    public:
        Particles simParticles;
        vector<vector<int>> indices;

        SegmentMesh();
        ~SegmentMesh();

        void setUpSizes(const int numParticles);
        void addSegment(int indexA, int indexB);
        void addSegments(int startingA, vector<int>& attachedBs);
    };

    class Face {
    public:
        vector<int> indices;
        vector<Face*> attachedFaces;
        int id;

        Face(vector<int> indicesInput, int inputId);
        ~Face();

        bool containsIndex(int i);
        bool containsIndices(int i, int j);
        bool containsOrderedIndices(int i, int j);

        bool adjacentToFaceById(int faceId);
        int getAdjacentFaceIndexByIndices(int sharedI, int sharedJ);
        bool adjacentToFace(Face* adjacentFace);
        bool shouldBeAdjacentToFace(Face* face);
        bool removeAdjacentFaceById(int id);
        Face* getAdjacentById(int faceId);

        int getNonListedIndex(int pi_index, int pj_index);

        void resetIndices(int index0, int index1, int index2);
    };

 
    class TriangleMesh {
    public:
        Particles simParticles;
        vector<Face*> faces;

        TriangleMesh();

        ~TriangleMesh();

        void setUpSizes(const int& numParticles);

        void calculateWeights(
            std::vector<int>& static_particles,
            Particles* simParticles);
        void calculateAdjacentFaces(const int& num_faces);
    };
}

#endif