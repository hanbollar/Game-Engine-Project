#pragma once

#include <set>
#include <SOIL.h>

#include "globals.h"
#include "file_loader.h"
#include "scene/drawable.h"
#include "typedefs.h"
#include "window_maintainer.h"

class ShaderProgram {
protected:
	set<HandleType> using_handles_;

	GLuint program_ = 0;

    unsigned int num_attributes = 0;

	// For binding and unbinding elements during the drawing process.
	virtual void BeforeDraw(std::shared_ptr<Drawable>& d, const glm::mat4& global_transform, WindowMaintainer* m = nullptr) = 0;
	virtual void AfterDraw(std::shared_ptr<Drawable>& d) = 0;

public:
	ShaderProgram(const char* vertex_file, const char* fragment_file);
	~ShaderProgram();

	// Creates buffers and sets up attribute referencing for
	// rendering the drawable with this program.
	virtual void CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle = -1) = 0;

	const void UseMe() const { glUseProgram(program_); }
	void UseMe() { glUseProgram(program_); }

	const bool UsingHandle(HandleType type) const { return using_handles_.find(type) != using_handles_.end(); }
	bool UsingHandle(HandleType type) { return using_handles_.find(type) != using_handles_.end(); }

	void SetUniformDouble(const float& val, const GLuint& handle);
	void SetUniformFloat(const float& val, const GLuint& handle);
	void SetUniformInt(const int& val, const GLuint& handle);
	void SetUniformVec3(const glm::vec3 &vector3, const GLuint& handle);
	void SetUniformVec4(const glm::vec4 &vector4, const GLuint& handle);
	void SetUniformMat4(const glm::mat4 &matrix, const GLuint& handle);
    void SetUniformSampler(const int& val, const GLuint& handle);

	void Draw(std::shared_ptr<Drawable>& d, const glm::mat4& global_transform, WindowMaintainer* m);

    static GLuint LoadShaderProgramFiles(const char* vertex_file_path, const char* fragment_file_path);
    static GLuint LoadShaderFile(const char* file_path, GLenum shader_type);
    static void LoadTextureFromFile(const char* file_path, GLuint& tex_id, bool createNewId);
};

