#pragma once

#include "globals.h"
#include "primitive.h"

class KDNode {
private:
    KDNode* left_;
    KDNode* right_;

    unsigned int axis_;

    const unsigned int MAX_NUM_ITEMS_AT_LEAF = 1;

    glm::vec3 min_bound_ = glm::vec3(INFINITY);
    glm::vec3 max_bound_ = glm::vec3(-INFINITY);
    std::vector<Primitive*> prims_;
public:
    KDNode();
    KDNode(const std::vector<Primitive*>& prims);
    KDNode(const std::vector<Primitive*>& prims, const glm::vec3& min_corner, const glm::vec3& max_corner);
    ~KDNode();

    void BuildWithPrimitives(const std::vector<Primitive*>& prims);

    bool WithinAnyShape(const glm::vec3& loc);
    bool IsLeaf();
};

class KDTree {
private:
    KDNode* root_ = nullptr;
public:
    KDTree();
    KDTree(const std::vector<Primitive*>& prims);
    ~KDTree();

    void BuildWithPrimitives(const std::vector<Primitive*>& prims);
    bool WithinAnyShape(const glm::vec3& loc);
};




