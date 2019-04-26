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

void Character::Jump(std::shared_ptr<Camera>& c) {

}

void Character::Crouch(std::shared_ptr<Camera>& c) {

}
