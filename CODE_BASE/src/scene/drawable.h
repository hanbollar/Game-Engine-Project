
#pragma once

#include <map>

#include "globals.h"
#include "typedefs.h"
#include "shader_programs/shader_program.h"

class ShaderProgram;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 uv;
    //glm::vec3 col;

    Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& tex/*, const glm::vec3& c*/)
        : pos(p), nor(n), uv(tex)//, col(c)
    {}
};

class Drawable {
protected:
	GLsizei count_ = 0;
	GLenum draw_mode_ = 0;

	map<HandleType, GLuint> using_handle_locations_ = {};

	vector<unsigned short> indices_;
    vector<Vertex> vertices_;

public:
    Drawable();
    Drawable(vector<Vertex>* pos, vector<unsigned short>* idx, const GLenum& draw_mode);
	~Drawable();

	vector<unsigned short>& indices() { return indices_; }
	vector<Vertex>& vertices() { return vertices_; }

	void SetHandleLocation(HandleType type, GLuint& location) {
		using_handle_locations_[type] = location;
	}
	const GLuint GetHandleLocation(HandleType type) const;
	GLuint GetHandleLocation(HandleType type);

    const bool UsingHandle(HandleType type) const { 
        return using_handle_locations_.find(type) != using_handle_locations_.end();
    }
    bool UsingHandle(HandleType type) {
        return using_handle_locations_.find(type) != using_handle_locations_.end();
    }

    void SetElementCount(const GLsizei& count) {
        count_ = count;
    }

    const GLsizei ElementCount() const { return count_; }
    GLsizei ElementCount() { return count_; }
    const GLsizei DrawMode() const { return draw_mode_; }
    GLsizei DrawMode() { return draw_mode_; }

    void ClearDrawable();

    void CreateBlankQuad();
};