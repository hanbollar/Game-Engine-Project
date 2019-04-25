#pragma once

#include "globals.h"
#include "scene_object.h"
#include "camera.h"

class Character : public SceneObject {
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

    float moving_magnitude_ = 1;

    void MoveInDirection(std::shared_ptr<Camera>& c, const glm::vec3& offset);
    void TurnInDirection(bool positive);
    void Jump(std::shared_ptr<Camera>& c);
    void Crouch(std::shared_ptr<Camera>& c);
};

