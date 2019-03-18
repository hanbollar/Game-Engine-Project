#pragma once

#include "globals.h"

class Triangle {
private:
	glm::vec3 positions_[3];
	glm::vec3 normals_[3];
	glm::vec2 uvs_[3];

public:
	Triangle();
	~Triangle();

	const glm::vec3* positions() const { return positions_; }
	glm::vec3* positions() { return positions_; }
	const glm::vec3* normals() const { return normals_; }
	glm::vec3* normals() { return normals_; }
	const glm::vec2* uvs() const { return uvs_; }
	glm::vec2* uvs() { return uvs_; }

	void SetPositions(glm::vec3& pos1, glm::vec3& pos2, glm::vec3& pos3);
	void SetNormals(glm::vec3& nor1, glm::vec3& nor2, glm::vec3& nor3);
	void SetUVs(glm::vec2& uv1, glm::vec2& uv2, glm::vec2& uv3);
};