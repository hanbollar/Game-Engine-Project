#pragma once

#include <set>
#include <SOIL.h>
#include <fstream>
#include <sstream>
#include <stb_image.h>

#include "typedefs.h"
#include "globals.h"
#include "scene/drawable.h"
#include "window_maintainer.h"

class Drawable;

class ShaderProgram {
protected:
	set<HandleType> using_handles_;

	GLuint program_ = -1;

    unsigned int num_attributes = 0;

public:
	ShaderProgram(const char* vertex_file, const char* fragment_file);
	~ShaderProgram();

	// Creates buffers and sets up attribute referencing for
	// rendering the drawable with this program.
	virtual void CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle = -1) = 0;

	const void UseMe() const { glUseProgram(program_); }
	void UseMe() { glUseProgram(program_); }

    const GLuint Handle() const { return program_; }
    GLuint Handle() { return program_; }

	const bool UsingHandle(HandleType type) const { return using_handles_.find(type) != using_handles_.end(); }
	bool UsingHandle(HandleType type) { return using_handles_.find(type) != using_handles_.end(); }
    
    // These are static - BUT all need to have glUseProgram called beforehand
    // for them to work as expected
	static void SetUniformDouble(const float& val, const GLuint& handle);
    static void SetUniformFloat(const float& val, const GLuint& handle);
	static void SetUniformInt(const int& val, const GLuint& handle);
	static void SetUniformVec3(const glm::vec3 &vector3, const GLuint& handle);
	static void SetUniformVec4(const glm::vec4 &vector4, const GLuint& handle);
	static void SetUniformMat4(const glm::mat4 &matrix, const GLuint& handle);
    static void SetUniformSampler(const int& val, const GLuint& handle);

    virtual void Draw(std::shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj) = 0;

    static GLuint LoadShaderProgramFiles(const char* vertex_file_path, const char* fragment_file_path);
    static GLuint LoadShaderFile(const char* file_path, GLenum shader_type);
    static void LoadTextureFromFile(const char* file_path, GLuint& tex_id, bool createNewId);
};

