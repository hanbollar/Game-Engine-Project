#ifdef PBD

#pragma once

#include "eigen_base.h"
#include "data.h"
#include "scene/drawable.h"
#include "constraints.h"

class Constraint;
class StretchConstraint;
class FaceBendingConstraint;
class BalloonVolumeConstraint;
class CollisionConstraint;
class Constraints;

namespace calculations {
    T calculateMeshVolume(vector<simulationData::Face*> &faces, DM &p);

    void dampVelocities_simple(DM& v, const int& numParticles);
    void dampVelocities(DM &x, DM &v, DM &w, const int& numParticles, const float& damping);

    void mathForFaceBendingConstraintCreation(simulationData::Face* f1, simulationData::Face* f2, vector<int> facesIndices, vector<V_horizontal> p, T& phi);
};

class PBDHandler {
private:
    bool start_sim_ = false;
    bool sim_completed_ = false;
    bool running_ = false;

    simulationData::Particles simParticles;
    std::vector<simulationData::Face*> faces;

    /*******SIMULATION VARIABLES*******/

    // originally defined
    int constraintIterations = 10;
    V gravity = V(T(0), T(-150), T(0));
    T deltaT = 1e-3;
    T damping = 0.05;
    T compressionStiffness = 1;
    T stretchingStiffness = 1;
    T bendingStiffness = 1;
    T restitution = 0.01; // used for friction
    vector<int> staticParticles = vector<int>();

    // calculated during simulation
    int numParticles = 0;
    int numFaces = 0;
    T originalVolume = 0;

    // not currently using
    T pressure = 1;
    T breakage = 1.5; // how much of the orig length before breaking the length

public:
    PBDHandler();
    ~PBDHandler();

    void SetSimObject(const char*);

    void Advect();
    void pbdSimulation();
};

#endif