#pragma once

#include "globals.h"

#include <fstream>
#include <sstream>

#include "triangle.h"

#include "../stb_image/stb_image.h"

void LoadObjectFile(
	//vector<std::shared_ptr<Triangle>>* triangles); // ,
	const char* file_path,
	vector<glm::vec3>* positions,
	vector<unsigned int>* position_indices,
	vector<glm::vec3>* normals,
	vector<unsigned int>* normal_indices,
	vector<glm::vec2>* uvs,
	vector<unsigned int>* uv_indices);

// TODO: load mtl file as texture file with materials