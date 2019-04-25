#include "billboard.h"

Billboard::Billboard() : SceneObject()
{}

Billboard::Billboard(
    const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : SceneObject(file_path, file_type, using_program, name, pos)
{}

Billboard::Billboard(
    std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : SceneObject(body, using_program, name, pos)
{}

Billboard::Billboard(const string& name, const glm::vec3& pos)
{}

Billboard::~Billboard()
{}
