#include "pbd_handler.h"

#ifdef PBD

/***************************************************/
/***************************************************/
/************ CALCULATIONS NAMESPACE ***************/
/***************************************************/
/***************************************************/

T calculations::calculateMeshVolume(vector<simulationData::Face*> &faces, DM &p) {
    T volume = 0;

    V_horizontal v1 = V_horizontal();
    V_horizontal v2 = V_horizontal();
    V_horizontal v3 = V_horizontal();

    for (int i = 0; i < (int)faces.size(); ++i) {

        v1 = p.row(faces[i]->indices[0]);
        v2 = p.row(faces[i]->indices[1]);
        v3 = p.row(faces[i]->indices[2]);

        v1 = v1.cross(v2);
        volume += dot_horizontal(v1, v3);
    }

    return abs(volume / T(6));
}

void calculations::dampVelocities_simple(DM& v, const int& numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        for (int j = 0; j < dim; ++j) {
            v(i, j) *= 0.98;
        }
    }
}

void calculations::dampVelocities(DM &x, DM &v, DM &w, const int& numParticles, const float& damping) {
    // calculate center of mass / center of velocity / overall mass
    DM m = DM(dim, numParticles);

    // since all weights are 1 or 0 --> will let all masses be the same
    // [need to update this if have diff weighted particles]
    m.row(0) = w.diagonal();
    m.row(1) = w.diagonal();
    m.row(2) = w.diagonal();

    DM xw = x * m;
    DM vw = v * m;
    V xwcm = V(xw.col(0).sum(), xw.col(1).sum(), xw.col(2).sum());
    V_horizontal vwcm = V_horizontal(vw.col(0).sum(), vw.col(1).sum(), vw.col(2).sum());
    T mSum = m.sum();
    xwcm /= mSum;
    vwcm /= mSum;

    // computing global linear velocity (L = sum ri x (mivi); I = sum ?????)
    V L = V(0, 0, 0);
    DM I = DM(dim, dim); I = I.setZero(dim, dim);
    // temp variables for computations
    DM I_temp = DM(dim, dim); I_temp = I_temp.Zero(dim, dim);
    V r = V();
    V xTranspose = V();
    V vTranspose = V();
    for (int i = 0; i < numParticles; ++i) {
        // since all weights are 1 or 0 --> will let all masses be the same as weights
        T mi = w(i, i);
        xTranspose = x.row(i).transpose();
        vTranspose = v.row(i).transpose();

        r = xTranspose - xwcm;
        L += r.cross(vTranspose) * mi;

        I_temp(0, 0) = T(0);  I_temp(0, 1) = -r(2); I_temp(0, 2) = r(1);
        I_temp(1, 0) = r(2);  I_temp(1, 1) = T(0);  I_temp(1, 2) = -r(0);
        I_temp(2, 0) = -r(1); I_temp(2, 1) = r(0);  I_temp(2, 2) = T(0);

        I += ((I_temp * I_temp.transpose()) * mi);
        I_temp = I_temp.Zero(dim, dim);
    }

    // calculate inverse of I
    T det = I.determinant();
    I.adjointInPlace();
    I /= det;

    V omega = I * L;
    for (int i = 0; i < numParticles; ++i) {
        r = x.row(i).transpose() - xwcm;
        v.row(i) += damping * (vwcm + omega.cross(r).transpose() - v.row(i));
    }
}

void calculations::mathForFaceBendingConstraintCreation(simulationData::Face* f1, simulationData::Face* f2, vector<int> facesIndices, vector<V_horizontal> p, T& phi) {
    // using same p ordering as in update for this constraint
    V n1 = (p[2] - p[0]).cross(p[3] - p[0]); n1.normalize();
    V n2 = (p[3] - p[1]).cross(p[2] - p[1]); n2.normalize();

    T d = clamp(n1.dot(n2), T(-1), T(1));
    if (testing && isnan(d)) { throw; }

    phi = acos(d);
    if (testing && abs(acos(d) - phi) < EPSILON) { return; }
}

/**************************************************/
/**************************************************/
/***************** PBD HANDLER ********************/
/**************************************************/
/**************************************************/

PBDHandler::PBDHandler() {
}

PBDHandler::~PBDHandler() {
}

void PBDHandler::Advect() {
    // TODO:: 
}

void PBDHandler::SetSimObject(const char* file_name) {
    string triangleFile = file_name;

    string line;
    ifstream myfile(triangleFile);

    vector<V> newPositions = vector<V>();

    int numPositions = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (!(line.size() > 0)) { continue; }

            bool vertex = false;
            bool face = false;
            bool loadLine = false;
            if (line.at(0) == 'v' && line.at(1) == ' ') {
                vertex = true;
                loadLine = true;
            }
            else if (line.at(0) == 'f' && line.at(1) == ' ') {
                face = true;
                loadLine = true;
            }

            if (loadLine) {
                line = line.substr(2);
                int first = line.find(' ', 0);
                T firstVal = stod(line.substr(0, first));
                line = line.substr(first + 1);
                int second = line.find(' ', 0);
                T secondVal = stod(line.substr(0, second));
                line = line.substr(second + 1);
                T thirdVal = stod(line);

                if (vertex) {
                    /// loading positions
                    newPositions.push_back(V(firstVal, secondVal, thirdVal));
                    ++numPositions;
                }
                else if (face) {
                    // reached faces part of obj - if never been initialized yet, initialize
                    // note : (positions must be initialized before faces bc of obj format)

                    /// loading faces
                    // obj files index from 1 but we index from 0.
                    firstVal -= 1;
                    secondVal -= 1;
                    thirdVal -= 1;

                    faces.push_back(new simulationData::Face({ (int)firstVal, (int)secondVal, (int)thirdVal }, faces.size()));
                }
            }
        }
        simParticles.addPositions(newPositions);
    }
}

void PBDHandler::pbdSimulation() {
    /// setup mesh values
    simulationData::TriangleMesh mesh = simulationData::TriangleMesh();

    // TODO::::----------------------------------------------
    //loadFromObj(mesh.simParticles, mesh.faces);
    mesh.simParticles.resizeVelocityAndMassesFromPositions();

    /// simulation variables
    numParticles = mesh.simParticles.numParticles;
    numFaces = mesh.faces.size();
    originalVolume = calculations::calculateMeshVolume(mesh.faces, mesh.simParticles.positions);
    staticParticles = {}; // fill this with the indices of the static vertices

    mesh.calculateWeights(staticParticles, &simParticles);
    mesh.calculateAdjacentFaces(numFaces);

    DM x = mesh.simParticles.positions;
    DM v = mesh.simParticles.velocities;
    DM w = mesh.simParticles.inverseMasses;

    // edge pairings only stored i,j s.t. i <= j
    // [removes the issue of checking based on duplicate edgings in the map - i,j and j,i]
    map<int, vector<int>> brokenEdges = map<int, vector<int>>();
    bool allowedToBreak = false;

    /// setup overall constraints
    Constraints allConstraints = Constraints();
    allConstraints.createStretchConstraints(mesh.faces, x, w, allowedToBreak, numParticles, stretchingStiffness, compressionStiffness);
    allConstraints.createFaceBendingConstraints(mesh.faces, x, w, allowedToBreak, numFaces, bendingStiffness);
    cout << "finished stretch, bending, volume constraints" << endl;

    /// temp variables
    DM p = DM(numParticles, numParticles);  // used to store updated position locations
    DM ONESnx1 = DM::Ones(numParticles, 1);     // empty variable used for calculations
    x.col(1) += ONESnx1 * T(4);                     // make mesh go above ground for beg of simulation // 7 plane 3.75 cow

    cout << "beginning simulation looping" << endl;
    //////////// BEGIN SIMULATION LOOP
    for (int simulationStep = 0; simulationStep < numFrames; ++simulationStep) {
        /// (5) for all vertices - update velocity by doing vi = vi + deltat*wi*fext(xi)
        ONESnx1 = DM::Ones(numParticles, 1);     // make sure it resizes for edge breakage
        v = v + deltaT * w * ONESnx1 * gravity.transpose();

        /// (6) dampVelocites(v1,...vN)
        calculations::dampVelocities(x, v, w, numParticles, damping);

        /// (7) for all verticies i find projected point assuming no collisions pi = xi + deltat vi
        p = x + deltaT * v;

        /// (8) for all velocities generate collision constraints (xi -> pi)
        allConstraints.createCollisionConstraints(p, w, numParticles);

        /// (9) loop the solver the number of desired iterations projecting the constraints
        for (int i = 0; i < constraintIterations; ++i) {
            allConstraints.update(p, brokenEdges);
        }

        /// (12) for all vertices update vi and xi for next overall loop simulation step
        v = (p - x) / deltaT;
        x = p;

        /// (16) velocities update (for friction etc - only on particles involved in this iterations collisions)
        // since collision constraints are the last to be updated in allConstraints update - dont need to reconstrain here
        allConstraints.updateVelocitiesOfCollisions(v, restitution);
        //allConstraints.updateRigidBodies(p, x);
        //allConstraints.updateEdges(brokenEdges, &x, &v, w, p, mesh.faces);

        // TODO::::::------------------------------------
        //center += centerVelocity; // moving sphere for collisions

        /// - here print out files of current simulation step
        mesh.simParticles.updateValues(x, v, w);

        // TODO::::::------------------------------------
        //writeToFileParticles(string(filePBDName + to_string(simulationStep)), mesh.simParticles);
        //writeToFileSegment(string(filePBDName + to_string(simulationStep)), mesh.simParticles, mesh.faces);
        //writeToFileTriangles(filePBDName + to_string(simulationStep), mesh);
        //writeToFileOnePosition(string("sphereCenter_" + to_string(simulationStep)), center); // for debugging sphere location
    }
    //////////// END SIMULATION LOOP
    cout << "simulation completed" << endl;
    sim_completed_ = true;
}

#endif