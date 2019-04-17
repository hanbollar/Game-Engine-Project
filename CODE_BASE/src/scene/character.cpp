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

void Character::MoveInDirection(std::shared_ptr<Camera>& camera, const glm::vec3& offset) {
    camera->ref = glm::vec3(
        glm::translate(glm::mat4(1.f), offset) * glm::vec4(camera->ref, 1.f)
    );
    camera->RecomputeAttributes();
    global_transform_ = glm::translate(global_transform_, offset);
}

void Character::Jump(std::shared_ptr<Camera>& c) {

}

void Character::Crouch(std::shared_ptr<Camera>& c) {

}
