#pragma once

#include <fstream>
#include <sstream>
#include <stb_image.h>

#include "third_party/tinyobj/tiny_obj_loader.h"

#include "globals.h"
#include "scene/triangle.h"

namespace FileLoader {

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


    std::string GetBaseDir(const std::string& filepath);

}