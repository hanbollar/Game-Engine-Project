#include "bone.h"

Bone::Bone() : local_transform_(glm::mat4(1.f)), parent_(nullptr)
{}

Bone::Bone(const glm::mat4& transf, Bone* p) : local_transform_(transf), parent_(p)
{}

glm::vec3 Bone::GetWorldPosition() {
    return glm::vec3(GetWorldTransform() * glm::vec4(0, 0, 0, 1));
}

glm::mat4 Bone::GetLocalTransform() {
    return local_transform_;
}
glm::mat4 Bone::GetWorldTransform() {
    if (!parent_) {
        return local_transform_;
    }
    return parent_->GetWorldTransform() * local_transform_;
}

glm::mat4 Bone::GetInverseWorldTransform() {
    return glm::inverse(GetWorldTransform());
}

glm::mat4 Bone::GetParentTransform() {
    return parent_->GetLocalTransform();
}

void Bone::SetLocalTransform(const glm::mat4& m) {
    local_transform_ = m;
}