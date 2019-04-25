#include "character.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL

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

void Character::MoveInDirection(std::shared_ptr<Camera>& camera, const glm::vec3& offset) {
    // camera update
    glm::mat4 transformation = global_transform_; // your transformation matrix.
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transformation, scale, rotation, translation, skew, perspective);
    rotation = glm::conjugate(rotation);
    glm::mat4 rot = glm::toMat4(rotation);

    global_transform_ = rot * glm::translate(glm::mat4(1.f), offset) * glm::inverse(rot) * global_transform_;

    camera->ref = glm::vec3(global_transform_ * glm::vec4(0, 0, 0, 1));
    camera->eye = glm::vec3(
        rot * glm::translate(glm::mat4(1.f), offset) * glm::inverse(rot) * glm::vec4(camera->eye, 1)
    );

    camera->RecomputeAttributes();
}

void Character::Jump(std::shared_ptr<Camera>& c) {

}

void Character::Crouch(std::shared_ptr<Camera>& c) {

}

void Character::TurnInDirection(bool positive) {
    global_transform_ = glm::rotate(global_transform_, positive ? float(M_PI/6.f) : float(-M_PI/6.f), glm::vec3(0, 1, 0));
}
