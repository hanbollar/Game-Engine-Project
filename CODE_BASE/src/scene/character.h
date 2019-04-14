#pragma once

#include "globals.h"
#include "mesh.h"

class Character
{
private:
    std::string name_;
    glm::mat4 global_transform_;
public:
    Character();
    Character(shared_ptr<Drawable> body_mesh, const string& name, const glm::vec3& pos);
    ~Character();

    shared_ptr<Drawable> character_body_;

    glm::vec3 GetGlobalPosition();
    glm::mat4 GetGlobalTransf_NoTranslation();
    glm::mat4 GetGlobalTransform();

    void MoveLeft();
    void MoveRight();
    void MoveForward();
    void MoveBackward();
    void Jump();
    void Crouch();
};

