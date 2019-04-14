#include "drawable.h"

Drawable::Drawable() :
    type_(VisualType::GenericMesh),
    positions_(vector<glm::vec3>()),
    normals_(vector<glm::vec3>()), uvs_(vector<glm::vec2>()),
    colors_(vector<glm::vec3>()), indices_(vector<unsigned int>()) {}

/*Drawable::Drawable() {
    // blah - TODO FINISH - WORK ON MAKING BILLBOARDS - SETUP SO BILLBOARDS HAVE OWN SEPARATE SHADER / BILLBOARD PROGRAM FOR THEM. THEY DONT EVEN NEED A MESH POSSIBLY.
    // OR MAYBE HAVE A SHADER FOR BUTTONS - LIKE SCREEN PRESSING BUTTONS. AND HAVE A WAY TO HANDLE THEM OUTSIDE THE SHADER.
}*/

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
    if (normals_.size() > 0) { normals_.clear(); }
    if (uvs_.size() > 0) { uvs_.clear(); }
    if (colors_.size() > 0) { colors_.clear(); }
    if (indices_.size() > 0) { indices_.clear(); }
    if (positions_.size() > 0) { positions_.clear(); }
}

void Drawable::MakeScreenQuad() {
    ClearDrawable();

    positions_.push_back(glm::vec3(-1.f, -1.f, 0.f));
    uvs_.push_back(glm::vec2(0.f, 0.f));

    positions_.push_back(glm::vec3(1.f, -1.f, 0.f));
    uvs_.push_back(glm::vec2(1.f, 0.f));

    positions_.push_back(glm::vec3(1.f, 1.f, 0.f));
    uvs_.push_back(glm::vec2(1.f, 1.f));

    positions_.push_back(glm::vec3(-1.f, 1.f, 0.f));
    uvs_.push_back(glm::vec2(0.f, 1.f));

    indices_.push_back(0);
    indices_.push_back(1);
    indices_.push_back(2);
    indices_.push_back(3);

    count_ = indices_.size();
    draw_mode_ = GL_QUADS;
}