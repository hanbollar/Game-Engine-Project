#include "mesh.h"

Mesh::Mesh() : Drawable() {
    CreateDefaultMesh();
}

Mesh::Mesh(const char* file_path, int file_type) : Drawable() {
    Load(file_path, file_type);
}

Mesh::~Mesh() {}

void Mesh::CreateDefaultMesh() {
	glm::vec3 A= glm::vec3(-1.f, -1.f, -1.f);
	glm::vec3 B= glm::vec3(-1.f, -1.f, 1.f);
	glm::vec3 C= glm::vec3(-1.f, 1.f, 1.f);
	glm::vec3 D= glm::vec3(1.f, 1.f, -1.f);
	glm::vec3 E= glm::vec3(-1.f, 1.f, -1.f);
	glm::vec3 F= glm::vec3(1.f, -1.f, 1.f);
	glm::vec3 G= glm::vec3(1.f, -1.f, -1.f);
	glm::vec3 H= glm::vec3(1.f, 1.f, 1.f);

	if (positions_.size() == 0) {
		positions_ = vector<glm::vec3>();
	}
	positions_.clear();
	positions_.push_back(A);
	positions_.push_back(B);
	positions_.push_back(C);
	positions_.push_back(D);
	positions_.push_back(E);
	positions_.push_back(F);
	positions_.push_back(G);
	positions_.push_back(H);

	vector<int> temp_indices = {
		0, 1, 2, 3, 0, 4, 5, 0, 6, 3, 6,
		0, 0, 2, 4, 5, 1, 0, 2, 1, 5, 7,
		6, 3, 6, 7, 5, 7, 3, 4, 7, 4, 2,
		7, 2, 5
	};
    for (int i = 0; i < temp_indices.size(); ++i)
	if (indices_.size() == 0) {
		indices_ = vector<unsigned int>();
	}
	indices_.clear();
	for (int i = 0; i < temp_indices.size(); ++i) {
		indices_.push_back(unsigned int(temp_indices[i]));
	}

	count_ = static_cast<GLsizei>(indices_.size());
	draw_mode_ = GL_TRIANGLES;
}

void Mesh::Load(const char* file_path, int file_type) {
	// TODO: (mesh load) allow for loading of different file types... right now only have 0 for obj.
    if (positions_.size() > 0) {
        positions_.clear();
    }
    if (indices_.size() > 0) {
        indices_.clear();
    }
    if (normals_.size() > 0) {
        normals_.clear();
    }
    if (uvs_.size() > 0) {
        uvs_.clear();
    }
    if (colors_.size() > 0) {
        colors_.clear();
    }

	if (file_type == 0) {
		// TODO: (mesh/file load) do we really need a set of indices for all these components?
		LoadObjectFile(file_path, &positions_, &indices_, &normals_, nullptr, &uvs_, nullptr);
	}

    count_ = static_cast<GLsizei>(indices_.size());
    draw_mode_ = GL_TRIANGLES;
}