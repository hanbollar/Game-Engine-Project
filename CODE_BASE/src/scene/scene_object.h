#pragma once

//#include "globals.h"
#include "drawable.h"
#include "shader_programs/shader_program.h"

class SceneObject {
private:
    void AssimpLoadObj(const char* file_path);
    void AssimpLoadCollada(const char* file_path);

protected:
   
    std::string name_;
    glm::mat4 global_transform_;

    std::shared_ptr<ShaderProgram> program_ptr_;
    GLuint program_;

    void Load(const char* file_path, Filetype mesh_file_type);

public:
    std::vector<std::shared_ptr<Drawable>> drawable_components_;

    SceneObject();
    SceneObject(
        std::vector<std::shared_ptr<Drawable>>* drawable_components, std::shared_ptr<ShaderProgram> using_program,
        const string& name, const glm::vec3& pos);
    SceneObject(
        const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
        const string& name, const glm::vec3& pos);
	~SceneObject();

    glm::vec3 GetGlobalPosition();
    glm::mat4 GetGlobalTransf_NoTranslation();
    glm::mat4 GetGlobalTransform();

    GLuint Program() { return program_; }
    const GLuint Program() const { return program_; }

    void Draw(const glm::mat4& view_proj);
};