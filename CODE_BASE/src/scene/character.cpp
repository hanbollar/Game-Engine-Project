#include "character.h"

Character::Character() : SceneObject()
{}

Character::Character(
    const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
    const string& name, const glm::vec3& pos)
    : SceneObject(file_path, file_type, using_program, name, pos)
{}

Character::Character(
    std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
    const string& name, const glm::vec3& pos)
    : SceneObject(body, using_program, name, pos)
{}

Character::Character(const string& name, const glm::vec3& pos)
{}

Character::~Character()
{}

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
