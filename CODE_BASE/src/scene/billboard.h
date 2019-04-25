#pragma once

#include "globals.h"
#include "scene_object.h"
#include "camera.h"

class Billboard : public SceneObject {
public:
    Billboard();
    Billboard(
        const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    Billboard(
        std::vector<shared_ptr<Drawable>>* body, std::shared_ptr<ShaderProgram> using_program,
        const char* name, const glm::vec3& pos);
    Billboard(const string& name, const glm::vec3& pos);
    ~Billboard();

    
};

