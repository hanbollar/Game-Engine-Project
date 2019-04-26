#pragma once

#include "globals.h"

#include "kd_tree.h"

class CollisionHandler {
private:
    KDTree* root_;
public:
    CollisionHandler();
    ~CollisionHandler();

    void Build(const std::vector<Primitive*>& prims);

    bool TestLoc(const glm::vec3& loc);
    bool Raymarch(const glm::vec3& origin, const glm::vec3& direction, const int& testing_iters = 100);
    bool HitMeshWithinDist(const glm::vec3& origin, const glm::vec3& direction);
};