#pragma once

#include "globals.h"
#include "scene_object.h"
#include "camera.h"

class SceneTitleHandler {
private:
                         
public:
    SceneTitleHandler();
    SceneTitleHandler(
        std::shared_ptr<ShaderProgram> using_program,
        const string& name);
    ~SceneTitleHandler();

    void Draw();
};

