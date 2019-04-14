
#pragma once

#include <map>

#include "globals.h"
#include "typedefs.h"

class Drawable {
protected:
	GLsizei count_ = 0;
	GLenum draw_mode_ = 0;

    const VisualType type_;

	map<HandleType, GLuint> using_handle_locations_ = {};

	glm::mat4 model_matrix_ = glm::mat4(1.f);

	vector<glm::vec3> positions_;
	vector<unsigned int> indices_;
	vector<glm::vec3> normals_;
	vector<glm::vec2> uvs_;
	vector<glm::vec3> colors_;

    void ClearDrawable();

public:
    Drawable();
	~Drawable();

	const vector<unsigned int>& indices() const { return indices_; }
	vector<unsigned int>& indices() { return indices_; }
	const vector<glm::vec3>& positions() const { return positions_; }
	vector<glm::vec3>& positions() { return positions_; }
	const vector<glm::vec3>& normals() const { return normals_; }
	vector<glm::vec3>& normals() { return normals_; }
	const vector<glm::vec2>& uvs() const { return uvs_; }
	vector<glm::vec2>& uvs() { return uvs_; }
	const vector<glm::vec3>& colors() const { return colors_; }
	vector<glm::vec3>& colors() { return colors_; }

	void SetHandleLocation(HandleType type, GLuint& location) {
		using_handle_locations_[type] = location;
	}
	const bool GetHandleLocation(HandleType type, GLuint* location) const;
	bool GetHandleLocation(HandleType type, GLuint* location);

    const bool UsingHandle(HandleType type) const { 
        return using_handle_locations_.find(type) != using_handle_locations_.end();
    }
    bool UsingHandle(HandleType type) {
        return using_handle_locations_.find(type) != using_handle_locations_.end();
    }

    void SetElementCount(const unsigned int& count) {
        count_ = count;
    }

    VisualType VisualType() { return type_; }

    const GLsizei ElementCount() const { return count_; }
    GLsizei ElementCount() { return count_; }
    const GLsizei DrawMode() const { return draw_mode_; }
    GLsizei DrawMode() { return draw_mode_; }

	const glm::mat4& model_matrix() const { return model_matrix_; }
	glm::mat4 model_matrix() { return model_matrix_; }
	void SetModelMatrix(const glm::mat4& model_matrix) {
		model_matrix_ = model_matrix;
	}

    void MakeScreenQuad();
};