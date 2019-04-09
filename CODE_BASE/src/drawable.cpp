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