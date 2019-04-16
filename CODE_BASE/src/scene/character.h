#pragma once

#include "globals.h"
#include "scene_object.h"

class Character : public SceneObject {
public:
    Character();
    Character(
        const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
        const string& name, const glm::vec3& pos);
    Character(
        std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
        const string& name, const glm::vec3& pos);
    Character(const string& name, const glm::vec3& pos);
    ~Character();

    void MoveLeft();
    void MoveRight();
    void MoveForward();
    void MoveBackward();
    void Jump();
    void Crouch();
};

