#include "collision_handler.h"

CollisionHandler::CollisionHandler() : root_(nullptr) {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::Build(const std::vector<Primitive*>& prims) {
    if (root_ != nullptr) {
        delete root_;
    }
        
    root_ = new KDTree(prims);
}

bool CollisionHandler::TestLoc(const glm::vec3& loc) {
    // technically the kd tree is built partially wrong with the primitives because I am not considering
    // them as full shapes, but as points when building the kdtree. since these are going to be rudimentary
    // shapes in my implementation I will be assuming that any scaling that occurs due to rotational skewing
    // is extremely minimal and that I will be changing them so slightly that this error becomes negligible
    // in terms of bounds checking
    return (root_->WithinAnyShape(loc));
}

bool CollisionHandler::Raymarch(const glm::vec3& origin, const glm::vec3& direction, const int& testing_iters) {
    float t = 0.5f;

    // looks until reaches testing_iters * t
    for (int i = 0; i < testing_iters; i++)  {
        glm::vec3 testCoord = origin + i*t*direction;

        // technically the kd tree is built partially wrong with the primitives because I am not considering
        // them as full shapes, but as points when building the kdtree. since these are going to be rudimentary
        // shapes in my implementation I will be assuming that any scaling that occurs due to rotational skewing
        // is extremely minimal and that I will be changing them so slightly that this error becomes negligible
        // in terms of bounds checking
        if (root_->WithinAnyShape(testCoord)) {
            return true;
        }
    }
    return false;
}

bool CollisionHandler::HitMeshWithinDist(const glm::vec3& origin, const glm::vec3& direction) {
    //const float COLLISIONRADIUS = 5.f;
    //std::vector<std::shared_ptr<Triangle>>& found_list = root->FindFacesWithinRadius(COLLISIONRADIUS);

    // for each face - if location is not in direction of movement (dot prod neg), ignore
    // otherwise check barycentric within for each triangular face

    // TODO: finish implementing maybe - might not need this method for current purposes.

    return false;
}