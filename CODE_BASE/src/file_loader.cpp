#include "file_loader.h"

void LoadObjectFile(
    const char* file_path,
    vector<glm::vec3>* positions,
    vector<unsigned int>* position_indices,
    vector<glm::vec3>* normals,
    vector<unsigned int>* normal_indices,
    vector<glm::vec2>* uvs,
    vector<unsigned int>* uv_indices) {

    if (!positions || !position_indices) {
        throw;
    }

    string line;
    FILE * file = fopen(file_path, "r");
    if (!file) {
        cout << "Could not open file: " << file_path << endl;
        throw;
    }

    while (true) {
        char lineHeader[200];
        if (fscanf(file, "%s", lineHeader) == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            positions->push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0 && uvs) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs->push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0 && normals) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals->push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                ErrorHandler::ThrowError("Cannot read file.");
                return;
            }

            position_indices->push_back(vertexIndex[0] - 1);
            position_indices->push_back(vertexIndex[1] - 1);
            position_indices->push_back(vertexIndex[2] - 1);

            if (normal_indices) {
                normal_indices->push_back(vertexIndex[0] - 1);
                normal_indices->push_back(vertexIndex[1] - 1);
                normal_indices->push_back(vertexIndex[2] - 1);
            }
            if (uv_indices) {
                uv_indices->push_back(vertexIndex[0] - 1);
                uv_indices->push_back(vertexIndex[1] - 1);
                uv_indices->push_back(vertexIndex[2] - 1);
            }

            /*std::shared_ptr<Triangle> tri = std::shared_ptr<Triangle>();
            tri->SetPositions(pos[vertexIndex[0]],
                pos[vertexIndex[1]],
                pos[vertexIndex[2]]);
            tri->SetNormals(nor[normalIndex[0]],
                nor[normalIndex[1]],
                nor[normalIndex[2]]);
            tri->SetUVs(tex[uvIndex[0]],
                tex[uvIndex[1]],
                tex[uvIndex[2]]);*/
        }
        int i = 0;
    }
    int j = 0;
}