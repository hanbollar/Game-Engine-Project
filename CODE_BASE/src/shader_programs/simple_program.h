#pragma once

#include "globals.h"
#include "shader_program.h"
#include "scene/scene.h"

class SimpleProgram : public ShaderProgram {
public:
    SimpleProgram(const char* vertex_file, const char* fragment_file);
    ~SimpleProgram();

    virtual void CreateDrawable(shared_ptr<Drawable>& mesh, GLuint texture_handle = -1);
    virtual void BeforeDraw(std::shared_ptr<Drawable>& d, const glm::mat4& global_transform, WindowMaintainer* m = nullptr);
    virtual void AfterDraw(std::shared_ptr<Drawable>& d);
};