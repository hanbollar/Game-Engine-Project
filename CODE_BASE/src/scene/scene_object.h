#pragma once

//#include "globals.h"
#include "drawable.h"
#include "shader_programs/shader_program.h"

class SceneObject {
private:
    void AssimpLoadObj(const char* file_path);
    void AssimpLoadCollada(const char* file_path);

protected:
    std::vector<std::shared_ptr<Drawable>> drawable_components_;
    std::string name_;
    glm::mat4 global_transform_;

    std::shared_ptr<ShaderProgram> program_;

    void Load(const char* file_path, Filetype mesh_file_type);

public:
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

    void Draw(const WindowMaintainer* m);
};