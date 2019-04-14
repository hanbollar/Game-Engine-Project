#include "triangle.h"

Triangle::Triangle() : positions_(), normals_(), uvs_() {}
Triangle::~Triangle() {}

void Triangle::SetPositions(glm::vec3& pos1, glm::vec3& pos2, glm::vec3& pos3) {
	positions_[0] = pos1;
	positions_[2] = pos2;
	positions_[3] = pos3;
}

void Triangle::SetNormals(glm::vec3& nor1, glm::vec3& nor2, glm::vec3& nor3) {
	normals_[0] = nor1;
	normals_[1] = nor2;
	normals_[2] = nor3;
}

void Triangle::SetUVs(glm::vec2& uv1, glm::vec2& uv2, glm::vec2& uv3) {
	uvs_[0] = uv1;
	uvs_[1] = uv2;
	uvs_[2] = uv3;
}
