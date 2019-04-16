#include "drawable.h"

Drawable::Drawable()
    : vertices_(vector<Vertex>()), indices_(vector<GLuint>()), draw_mode_(GL_TRIANGLES), count_(0)
{}

Drawable::Drawable(vector<Vertex>* vertices, vector<GLuint>* idx, const GLenum& draw_mode)
    : vertices_(*vertices), indices_(*idx), draw_mode_(draw_mode), count_(static_cast<GLsizei>(idx->size()))
{}

Drawable::~Drawable() {
	GLuint location = 0;
    auto handle_iterator = using_handle_locations_.begin();
	if (GetHandleLocation(HandleType::VAO, &location)) {
		glDeleteVertexArrays(1, &location);
        ++handle_iterator;
	}
    if (GetHandleLocation(HandleType::TEX, &location)) {
        glDeleteTextures(1, &location);
        ++handle_iterator;
    }
	while (handle_iterator != using_handle_locations_.end()) {
		location = handle_iterator->second;
		glDeleteBuffers(1, &location);
		++handle_iterator;
	}
}

const bool Drawable::GetHandleLocation(HandleType type, GLuint* location) const {
	try {
		*location = using_handle_locations_.at(type);
		return true;
	} catch (out_of_range) {
		return false;
	}
}

bool Drawable::GetHandleLocation(HandleType type, GLuint* location) {
	try {
		*location = using_handle_locations_.at(type);
		return true;
	} catch (out_of_range) {
		return false;
	}
}

void Drawable::ClearDrawable() {
    if (indices_.size() > 0) { indices_.clear(); }
    if (vertices_.size() > 0) { vertices_.clear(); }
}