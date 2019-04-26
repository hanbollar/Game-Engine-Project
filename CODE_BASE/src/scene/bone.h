#pragma once

#include "globals.h"

#include "drawable.h"
#include "scene_object.h"

class Bone {
private:
    glm::mat4 local_transform_;
    Bone* parent_;

public:
    Bone();
    Bone(const glm::mat4& transf, Bone* parent);
    ~Bone();

    Bone* GetParent() { return parent_; }

    glm::vec3 GetWorldPosition();

    glm::mat4 GetLocalTransform();
    glm::mat4 GetWorldTransform();
    glm::mat4 GetInverseWorldTransform();
    glm::mat4 GetParentTransform();

    void SetLocalTransform(const glm::mat4& m);
};