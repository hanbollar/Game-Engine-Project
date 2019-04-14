#include "mesh.h"

Mesh::Mesh() : Drawable() {
    CreateDefaultMesh();
}

Mesh::Mesh(const char* file_path, Filetype file_type) : Drawable() {
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



void Mesh::Load(const char* file_path, Filetype mesh_file_type) {
	// TODO: (mesh load) allow for loading of different file types... right now only have 0 for obj.
    ClearDrawable();

	if (mesh_file_type == Filetype::OBJ) {
		// TODO: (mesh/file load) do we really need a set of indices for all these components?
		FileLoader::LoadObjectFile(file_path, &positions_, &indices_, &normals_, nullptr, &uvs_, nullptr);

        
        /*std::string base_dir = FileLoader::GetBaseDir(file_path);
        if (base_dir.empty()) {
            base_dir = ".";
        }
#ifdef _WIN32
        base_dir += "\\";
#else
        base_dir += "/";
#endif
        std::string warn;
        std::string err;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path,
            base_dir.c_str());
        
        
        std::string errors = tinyobj::LoadObj(shapes, materials, file_path);
        std::cout << errors << std::endl;
        if (errors.size() == 0) {
            //Read the information from the vector of shape_ts
            // TODO: DO FOR MORE THAN ONE SHAPE;
            for (unsigned int i = 0; i < 1; /* shapes.size();* i++) {
                std::vector<float> &positions = shapes[i].mesh.positions;
                std::vector<float> &normals = shapes[i].mesh.normals;
                std::vector<float> &uvs = shapes[i].mesh.texcoords;
                std::vector<unsigned int> &indices = shapes[i].mesh.indices;

                for (unsigned int ui : indices) {
                    indices_.push_back(ui);
                }

                bool normalsExist = normals.size() > 0;
                for (int x = 0; x < positions.size(); x += 3) {
                    positions_.push_back(glm::vec3(positions[x], positions[x + 1], positions[x + 2]));
                    if (normalsExist) {
                        normals_.push_back(glm::vec3(normals[x], normals[x + 1], normals[x + 2]));
                    }
                }

                if (bool uvsExist = uvs.size() > 0) {
                    for (int x = 0; x < uvs.size(); x += 2) {
                        uvs_.push_back(glm::vec2(uvs[x], uvs[x + 1]));
                    }
                }

                /*
                generateIdx();
                context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
                context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, glIndices.size() * sizeof(GLuint), glIndices.data(), GL_STATIC_DRAW);

                generatePos();
                context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
                context->glBufferData(GL_ARRAY_BUFFER, glPos.size() * sizeof(glm::vec4), glPos.data(), GL_STATIC_DRAW);

                if (normalsExist)
                {
                    generateNor();
                    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
                    context->glBufferData(GL_ARRAY_BUFFER, glNor.size() * sizeof(glm::vec4), glNor.data(), GL_STATIC_DRAW);
                }

                if (uvsExist)
                {
                    generateUV();
                    context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
                    context->glBufferData(GL_ARRAY_BUFFER, glUV.size() * sizeof(glm::vec2), glUV.data(), GL_STATIC_DRAW);
                }

                count += indices.size();
                *
            }
        }
        else {
            //An error loading the OBJ occurred!
            std::cout << errors << std::endl;
        }*/
	}

    count_ = static_cast<GLsizei>(indices_.size());
    draw_mode_ = GL_TRIANGLES;
}

