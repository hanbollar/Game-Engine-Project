#pragma once

#include "globals.h"
#include "scene_object.h"
#include "rig_solver.h"
#include "bone.h"

class Character : public SceneObject {
    RigSolver rig_solver_;

    std::vector<Bone*> bones_;
    std::shared_ptr<SceneObject> bone_obj_vis_;
public:
    Character();
    Character(
        const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    Character(
        std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    Character(const char* name, const glm::vec3& pos);
    ~Character();

    void Jump(std::shared_ptr<Camera>& c);
    void Crouch(std::shared_ptr<Camera>& c);

    // temp
    void SetBones(std::vector<Bone*> bones);
    void SetBoneObjectVisual(std::shared_ptr<SceneObject> bone_obj_vis) {
        bone_obj_vis_ = bone_obj_vis;
    }
    void DrawBones(const glm::mat4& view_proj);

    virtual void Draw(const glm::mat4 view_proj);
    void HoldStaff(const glm::vec3& loc);
};

