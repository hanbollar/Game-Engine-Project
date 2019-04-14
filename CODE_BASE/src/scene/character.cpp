#include "character.h"

Character::Character()
    : character_body_(nullptr), name_(""), global_transform_(glm::mat4(1.f))
{}

Character::Character(shared_ptr<Drawable> body_mesh, const string& name, const glm::vec3& pos)
    : character_body_(body_mesh), name_(name), global_transform_(glm::translate(glm::mat4(1.f), pos))
{}

Character::~Character() {
}

glm::vec3 Character::GetGlobalPosition() {
    return glm::vec3(global_transform_[3]);
}

glm::mat4 Character::GetGlobalTransf_NoTranslation() {
    glm::mat4 returning = global_transform_;
    returning[3][0] = 0;
    returning[3][1] = 0;
    returning[3][2] = 0;
    return returning;
}

glm::mat4 Character::GetGlobalTransform() {
    return global_transform_;
}

void Character::MoveLeft() {
    global_transform_ = glm::translate(global_transform_, glm::vec3(-1, 0, 0));
}

void Character::MoveRight() {
    global_transform_ = glm::translate(global_transform_, glm::vec3(1, 0, 0));
}

void Character::MoveForward() {
    global_transform_ = glm::translate(global_transform_, glm::vec3(0, 0, 1));
}

void Character::MoveBackward() {
    global_transform_ = glm::translate(global_transform_, glm::vec3(0, 0, -1));
}

void Character::Jump() {

}

void Character::Crouch() {

}
