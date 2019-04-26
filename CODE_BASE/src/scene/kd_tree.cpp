#include "kd_tree.h"

KDTree::KDTree() : root_(nullptr) {}

KDTree::KDTree(const std::vector<Primitive*>& prims) : root_(new KDNode(prims))
{}

KDTree::~KDTree() {
    delete root_;
}

bool KDTree::WithinAnyShape(const glm::vec3& loc) {
    // just testing one position, not testing whole user_character_figure_
    // not optimal for implementation but rudimentary enough to work for now
    // TODO: optimize this better

    return root_->WithinAnyShape(loc);
}

void KDTree::BuildWithPrimitives(const std::vector<Primitive*>& prims) {
    if (root_ == nullptr) {
        root_ = new KDNode(prims);
    } else {
        delete root_;
        root_ = new KDNode(prims);
    }
}

/**************/

KDNode::KDNode() : min_bound_(glm::vec3(0)), max_bound_ (glm::vec3(0)), left_(nullptr), right_(nullptr) {}

KDNode::KDNode(const std::vector<Primitive*>& prims) : left_(nullptr), right_(nullptr) {
    for (Primitive* p : prims) {
        glm::vec3 temp_loc(p->GetGlobalTransform() * glm::vec4(0, 0, 0, 1));

        min_bound_.x = glm::min(min_bound_.x, temp_loc.x);
        min_bound_.y = glm::min(min_bound_.y, temp_loc.y);
        min_bound_.z = glm::min(min_bound_.z, temp_loc.z);

        max_bound_.x = glm::max(max_bound_.x, temp_loc.x);
        max_bound_.y = glm::max(max_bound_.y, temp_loc.y);
        max_bound_.z = glm::max(max_bound_.z, temp_loc.z);
    }
    
    if (prims.size() == MAX_NUM_ITEMS_AT_LEAF) {
        prims_ = prims;
    } else {
        BuildWithPrimitives(prims);
    }
}

KDNode::KDNode(const std::vector<Primitive*>& prims, const glm::vec3& min_corner, const glm::vec3& max_corner)
    : min_bound_(min_corner), max_bound_(max_corner) {

    // invalid setup check
    for (int i = 0; i < 3; ++i) {
        if (min_corner[i] > max_corner[i]) {
            ErrorHandler::ThrowError("Invalid min, max corner setup for KDNode");
        }
    }

    if (prims.size() == MAX_NUM_ITEMS_AT_LEAF) {
        prims_ = prims;
    } else {
        BuildWithPrimitives(prims);
    }
}

KDNode::~KDNode() {
    prims_.clear();
    delete left_;
    delete right_;
}

void KDNode::BuildWithPrimitives(const std::vector<Primitive*>& prims) {
    // already guaranted that this is false: (prims.size() <= MAX_NUM_ITEMS_AT_LEAF)

    // split longest axis in half
    if (max_bound_.x < min_bound_.x || max_bound_.y < min_bound_.y || max_bound_.z < min_bound_.z) {
        ErrorHandler::ThrowError("Improper configuration in KDNode.");
    }

    glm::vec3 diff = abs(max_bound_ - min_bound_);
    axis_ = (diff.x > diff.y && diff.x > diff.z) ? 0 : (diff.y > diff.x && diff.y > diff.z) ? 1 : 2;

    int split_index = prims.size() / 2.f;

    std::vector<Primitive*> left_prims(prims.begin(), prims.begin() + split_index);
    std::vector<Primitive*> right_prims(prims.begin() + split_index, prims.end());

    if (left_prims.size() > 0) {
        left_ = new KDNode(left_prims);
    }
    if (right_prims.size() > 0) {
        right_ = new KDNode(right_prims);
    }
}

bool KDNode::WithinAnyShape(const glm::vec3& loc) {
    if (IsLeaf()) {
        for (Primitive* p : prims_) {
            if (p->Within(loc)) {
                return true;
            }
        }
        return false;
    }

    if (loc.x <= this->min_bound_.x || loc.x >= this->max_bound_.x
        || loc.y <= this->min_bound_.y || loc.y >= this->max_bound_.y
        || loc.z <= this->min_bound_.z || loc.z >= this->max_bound_.z) {
        
        // point not within this search area
        return false;
    }

    return left_->WithinAnyShape(loc) || right_->WithinAnyShape(loc);
}

bool KDNode::IsLeaf() {
    return prims_.size() != 0;
}


