#pragma once

#include "globals.h"
#include "drawable.h"
#include "shader_programs/shader_program.h"
#include "camera.h"

//#define BONE 

class SceneObject {
private:
    void AssimpLoadObj(const char* file_path);
    void AssimpLoadCollada(const char* file_path);
#ifdef BONE
    void AssimpLoadRiggedCollada(const char* file_path);
#endif BONE

protected:
    std::string name_;
    glm::mat4 global_transform_;

    std::vector<glm::mat4> multiple_transforms_;

    std::shared_ptr<ShaderProgram> program_ptr_;
    GLuint program_;

    void Load(const char* file_path, Filetype mesh_file_type);

public:
    std::vector<std::shared_ptr<Drawable>> drawable_components_;
    std::vector<std::shared_ptr<SceneObject>> attached_components_;

    SceneObject();
    SceneObject(
        std::vector<std::shared_ptr<Drawable>>* drawable_components, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    SceneObject(
        const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    SceneObject(
        std::shared_ptr<Drawable> drawable_component, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
	~SceneObject();

    glm::vec3 GetGlobalPosition();
    glm::mat4 GetGlobalTransf_NoTranslation();
    glm::mat4 GetGlobalTransform();

    void SetGlobalTransform(const glm::mat4& transf);
    void AddGlobalTransform(const glm::mat4& transf);

    void SetTexture(GLuint tex_id);
    void SetTextures(GLuint tex_id1, GLuint tex_id2_opacity);

    void SetDrawMode(const GLenum& e);

    GLuint Program() { return program_; }
    const GLuint Program() const { return program_; }

    void CreateSelf();
    void Draw(const glm::mat4 view_proj = glm::mat4(1.f));

    virtual glm::vec3 MoveInDirection(std::shared_ptr<Camera>& camera, const glm::vec3& offset);
    virtual void TurnInDirection(bool positive);
};