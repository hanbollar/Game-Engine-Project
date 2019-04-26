#pragma once

#include "globals.h"
#include "Bone.h"

class RigSolver {
private:
    glm::vec3 target_;

    // solver constants
    const int& NUM_ITER = 50;
    const float& THRESHOLD = 0.05f;
    const int& NUM_ITER_BONES = 3;

public:
    RigSolver();
    ~RigSolver();

    void SetTarget(const glm::vec3& target) { target_ = target; }
    void SolveCurrent(Bone* endEffector);
};
