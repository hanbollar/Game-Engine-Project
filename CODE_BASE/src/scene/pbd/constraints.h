#ifdef PBD

#pragma once

#include "eigen_base.h"
#include "data.h"
#include "pbd_handler.h"

class PBDHandler;

namespace mapCalculations {
    bool mapContainsKeyValuePair(int i, int j, map<int, vector<int>>& map1);
    void addToMap(int i, int j, map<int, vector<int>>& map1);
};

class Constraint {
protected:
    bool remove;
    bool allowedToBreak;
public:
    Constraint(bool inputAllowedToBreak);
    ~Constraint();

    virtual void update(DM& p, map<int, vector<int>>& broken);
    bool broken();
};

class StretchConstraint : public Constraint {
protected:
    /// parental items
    // bool remove;
    // bool allowedToBreak;
public:
    int pi_index;
    int pj_index;

    T originalLength;
    T wi;
    T wj;

    T stretchingStiffness;
    T compressionStiffness;

    StretchConstraint(T inputLen, T inputwi, T inputwj, int inputpi_index, int inputpj_index, bool inputAllowedToBreak,
        T inputStretchingStiffness, T inputCompressionStiffness);
    ~StretchConstraint();

    virtual void update(DM& p, map<int, vector<int>>& broken, const int& constraintIterations);
};

class FaceBendingConstraint : public Constraint {
protected:
    /// parental items
    // bool remove;
    // bool allowedToBreak;
public:
    // notEdge, edge, edge, notEdge
    vector<int> facesIndices;
    // weights for each of these vertices
    vector<T> w;
    // original angle between the faces
    T phi;
    T stiffness;

    FaceBendingConstraint(vector<int> inputFacesIndices, vector<T> inputW, T inputPhi, bool inputAllowedToBreak, T inputStiffness);
    ~FaceBendingConstraint();

    virtual void update(DM& p, map<int, vector<int>>& broken, const int& constraintIterations);
};

class BalloonVolumeConstraint : public Constraint {
protected:
    /// parental items
    // bool remove;
    // bool allowedToBreak;
public:
    BalloonVolumeConstraint(bool inputAllowedToBreak);
    ~BalloonVolumeConstraint();

    virtual void update(DM& p, map<int, vector<int>>& broken);
};

class CollisionConstraint {
public:
    // index of the position needing to be updated
    int index;
    bool ground;
    bool sphere;
    V_horizontal qc;
    V_horizontal nc;
    T w;

    CollisionConstraint(int inputIndex, bool inputGround, bool inputSphere, T inputWeight);
    ~CollisionConstraint();

    void update(DM& p);
};

class Constraints {
public:
    vector<Constraint*> stretchBendVolume;
    vector<CollisionConstraint*> collisions;

    Constraints();
    ~Constraints();

    void update(DM& p, map<int, vector<int>>& broken);

    void createStretchConstraints(vector<simulationData::Face*> &faces, DM &positions, DM& w, bool allowedToBreak, const int& numParticles, const float& stretchStiffness, const float& compressionStiffness);
    void createFaceBendingConstraints(vector<simulationData::Face*>& faces, DM& p, DM& w, bool allowedToBreak, const int& numFaces, const float& bendingStiffness);
    void createBalloonVolumeConstraints(DM &p, vector<vector<int>>& faces, bool allowedToBreak);
    void createCollisionConstraints(DM& p, DM& w, const int& numParticles);

    void updateVelocitiesOfCollisions(DM &v, const float& restitution);
    void updateRigidBodies(DM& p, DM& x);
    void updateEdges(map<int, vector<int>>& broken, DM* x, DM* v, DM& w, DM& p, vector<simulationData::Face*>& faces);
};

#endif
