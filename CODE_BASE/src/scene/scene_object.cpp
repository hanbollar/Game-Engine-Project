#include "scene_object.h"

SceneObject::SceneObject() {}

SceneObject::SceneObject(
    const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
    const string& name, const glm::vec3& pos)
    : drawable_components_(std::vector<std::shared_ptr<Drawable>>()),
      program_ptr_(using_program), program_(using_program->Handle()),
      global_transform_(glm::translate(glm::mat4(1.f), pos)), name_(name) {

    Load(file_path, file_type);
}

SceneObject::SceneObject(
    std::vector<std::shared_ptr<Drawable>>* drawable_components, std::shared_ptr<ShaderProgram> using_program,
    const string& name, const glm::vec3& pos)
    : drawable_components_(*drawable_components),
      program_ptr_(using_program), program_(using_program->Handle()),
      global_transform_(glm::translate(glm::mat4(1.f), pos)), name_(name)
{}

SceneObject::~SceneObject() {
    drawable_components_.clear();
}

std::string GetBaseDir(const std::string& filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

/***********************************************************/
/****************** LOADING FILES AND DATA *****************/
/***********************************************************/

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void SceneObject::AssimpLoadObj(const char* file_path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_GenNormals);
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
        ErrorHandler::ThrowError("AssimpLoadObj error loading file: " + std::string(importer.GetErrorString()));
    }

    glm::vec3 color(0.f);
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* assimpMesh = scene->mMeshes[i];

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        color = glm::vec3(rand() % 255, rand() % 255, rand() % 255);
        color /= 255.f;
        for (unsigned int j = 0; j < assimpMesh->mNumVertices; ++j) {
            vertices.push_back(Vertex(
                glm::vec3(assimpMesh->mVertices[j].x, assimpMesh->mVertices[j].y, assimpMesh->mVertices[j].z),
                glm::vec3(assimpMesh->mNormals[j].x, assimpMesh->mNormals[j].y, assimpMesh->mNormals[j].z),
                glm::vec2(assimpMesh->mTextureCoords[0][j].x, assimpMesh->mTextureCoords[0][j].y)
            ));
        }

        for (unsigned int j = 0; j < assimpMesh->mNumFaces; ++j) {
            indices.push_back(assimpMesh->mFaces[i].mIndices[0]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[1]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[2]);
        }

        drawable_components_.push_back(std::shared_ptr<Drawable>(new Drawable(&vertices, &indices, GL_TRIANGLES)));
    }
}

void SceneObject::AssimpLoadCollada(const char* file_path) {
    //Assimp::Importer importer;

    // TODO: ASSIMP LOAD COLLADA - IMPLEMENT ME
}

void SceneObject::Load(const char* file_path, Filetype mesh_file_type) {
    drawable_components_.clear();

    if (mesh_file_type == Filetype::OBJ) {
        AssimpLoadObj(file_path);
    } else if (mesh_file_type == Filetype::COLLADA) {
        AssimpLoadCollada(file_path);
    }

    if (!program_) {
        return;
    }
    for (std::shared_ptr<Drawable> d : drawable_components_) {
        GLuint texture_handle = -1; // NEED BETTER WAY TO HANDLE TEXTURE
        program_ptr_->CreateDrawable(d, texture_handle); //----------------------this is where issue is.
    }
}

/***********************************************************/
/********* POSITION AND TRANSFORMATION INFORMATION *********/
/***********************************************************/

glm::vec3 SceneObject::GetGlobalPosition() {
    return glm::vec3(global_transform_[3]);
}

glm::mat4 SceneObject::GetGlobalTransf_NoTranslation() {
    glm::mat4 returning = global_transform_;
    returning[3][0] = 0;
    returning[3][1] = 0;
    returning[3][2] = 0;
    return returning;
}

glm::mat4 SceneObject::GetGlobalTransform() {
    return global_transform_;
}

void SceneObject::Draw(const glm::mat4& view_proj) {
    for (std::shared_ptr<Drawable> d : drawable_components_) {
        program_ptr_->Draw(d, global_transform_, view_proj);
    }
}



