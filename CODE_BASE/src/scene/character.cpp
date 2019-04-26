#include "character.h"

Character::Character() : SceneObject()
{}

Character::Character(
    const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : SceneObject(file_path, file_type, using_program, name, pos)
{}

Character::Character(
    std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : SceneObject(body, using_program, name, pos)
{}

Character::Character(const char* name, const glm::vec3& pos)
{}

Character::~Character()
{}

void Character::Jump(std::shared_ptr<Camera>& c) { /* doesnt do anything atm */ }

void Character::Crouch(std::shared_ptr<Camera>& c) { /* doesnt do anything atm */ }

void Character::SetBones(std::vector<Bone*> bones) {
    bones_ = bones;
}

void Character::Draw(const glm::mat4 view_proj) {
    if (multiple_transforms_.size() == 0) {
        for (std::shared_ptr<Drawable> d : drawable_components_) {
            program_ptr_->Draw(d, global_transform_, view_proj);
        }

        for (std::shared_ptr<SceneObject> s : attached_components_) {
            s->Draw(view_proj * global_transform_);
        }

        return;
    }

    for (glm::mat4 m : multiple_transforms_) {
        for (std::shared_ptr<Drawable> d : drawable_components_) {
            program_ptr_->Draw(d, m, view_proj);
        }

        for (std::shared_ptr<SceneObject> s : attached_components_) {
            s->Draw(view_proj * m);
        }
    }
}

void Character::DrawBones(const glm::mat4& view_proj) {
    if (multiple_transforms_.size() == 0) {
        // --- added for testing
        for (Bone* b : bones_) {
            bone_obj_vis_->SetGlobalTransform(b->GetWorldTransform());
            bone_obj_vis_->Draw(view_proj * global_transform_);
        }
        return;
    }
    for (glm::mat4 m : multiple_transforms_) {
        // -- added for testing
        for (Bone* b : bones_) {
            bone_obj_vis_->SetGlobalTransform(b->GetWorldTransform());
            bone_obj_vis_->Draw(m);
        }
    }
}

void Character::HoldStaff(const glm::vec3& loc) {
    rig_solver_.SetTarget(loc);

    // update IK elements? or just through the shader?

    // right now hard coding to check works
    rig_solver_.SolveCurrent(bones_[bones_.size() - 1]);
}
