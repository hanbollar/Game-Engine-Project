#pragma once

#include "globals.h"
#include "shader_program.h"
#include "scene/drawable.h"

class DefaultProgram : public ShaderProgram {
public:
	DefaultProgram(const char* vertex_file, const char* fragment_file);
	~DefaultProgram();

	virtual void CreateDrawable(shared_ptr<Drawable>& mesh, GLuint texture_handle = -1);
    virtual void Draw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj);

    void CreateFrameBufferTexture();
};