#include "drawable.h"

Drawable::Drawable()
    : vertices_(vector<Vertex>()), indices_(vector<int>()), draw_mode_(GL_TRIANGLES), count_(0)
{}

Drawable::Drawable(vector<Vertex>* vertices, vector<int>* idx, const GLenum& draw_mode)
    : vertices_(*vertices), indices_(*idx), draw_mode_(draw_mode), count_(static_cast<GLsizei>(idx->size()))
{}

Drawable::~Drawable() {
	GLuint location = 0;
    auto handle_iterator = using_handle_locations_.begin();
	if (GetHandleLocation(HandleType::VAO) != -1) {
		glDeleteVertexArrays(1, &location);
        ++handle_iterator;
	}
    if (GetHandleLocation(HandleType::TEX) != -1) {
        glDeleteTextures(1, &location);
        ++handle_iterator;
    }
	while (handle_iterator != using_handle_locations_.end()) {
		location = handle_iterator->second;
		glDeleteBuffers(1, &location);
		++handle_iterator;
	}
}

const GLuint Drawable::GetHandleLocation(HandleType type) const {
	try {
		return using_handle_locations_.at(type);
	} catch (out_of_range) {
		return -1;
	}
}

GLuint Drawable::GetHandleLocation(HandleType type) {
	try {
		return using_handle_locations_.at(type);
	} catch (out_of_range) {
		return -1;
	}
}

void Drawable::ClearDrawable() {
    if (indices_.size() > 0) { indices_.clear(); }
    if (vertices_.size() > 0) { vertices_.clear(); }
}

void Drawable::CreateBlankQuad() {
    ClearDrawable();

    glm::vec3 nor(0, 0, -1);
    vertices_.push_back(Vertex(glm::vec3(-1, 1, 0), nor, glm::vec2(0, 1)));
    vertices_.push_back(Vertex(glm::vec3(-1,-1, 0), nor, glm::vec2(0, 0)));
    vertices_.push_back(Vertex(glm::vec3( 1, 1, 0), nor, glm::vec2(1, 1)));
            
    vertices_.push_back(Vertex(glm::vec3( 1,-1, 0), nor, glm::vec2(1, 0)));
    vertices_.push_back(Vertex(glm::vec3( 1, 1, 0), nor, glm::vec2(1, 1)));
    vertices_.push_back(Vertex(glm::vec3(-1,-1, 0), nor, glm::vec2(0, 0)));

    for (int i = 0; i < 6; ++i) {
        indices_.push_back(i);
    }
}