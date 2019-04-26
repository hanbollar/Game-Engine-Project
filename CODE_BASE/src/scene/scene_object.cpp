#include "scene_object.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL


SceneObject::SceneObject() {}

SceneObject::SceneObject(
    const char* file_path, Filetype file_type, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : drawable_components_(std::vector<std::shared_ptr<Drawable>>()),
      program_ptr_(using_program), program_(using_program->Handle()),
      global_transform_(glm::translate(glm::mat4(1.f), pos)), name_(name) {

    Load(file_path, file_type);
}

SceneObject::SceneObject(
    std::vector<std::shared_ptr<Drawable>>* drawable_components, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : drawable_components_(*drawable_components),
      program_ptr_(using_program), program_(using_program->Handle()),
      global_transform_(glm::translate(glm::mat4(1.f), pos)), name_(name)
{}

SceneObject::SceneObject(
    std::shared_ptr<Drawable> drawable_component, std::shared_ptr<ShaderProgram> using_program,
    const char* name, const glm::vec3& pos)
    : drawable_components_({ drawable_component }),
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

    const aiScene* scene = importer.ReadFile(file_path,
        aiProcess_GenSmoothNormals |
        aiProcess_GenUVCoords |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        //aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
        ErrorHandler::ThrowError("AssimpLoadObj error loading file: " + std::string(importer.GetErrorString()));
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* assimpMesh = scene->mMeshes[i];

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<int> indices = std::vector<int>();

        for (unsigned int j = 0; j < assimpMesh->mNumVertices; ++j) {
            vertices.push_back(Vertex(
                glm::vec3(assimpMesh->mVertices[j].x, assimpMesh->mVertices[j].y, assimpMesh->mVertices[j].z),
                glm::vec3(assimpMesh->mNormals[j].x, assimpMesh->mNormals[j].y, assimpMesh->mNormals[j].z),
                glm::vec2(assimpMesh->mTextureCoords[0][j].x, assimpMesh->mTextureCoords[0][j].y)
            ));
        }

        for (unsigned int j = 0; j < assimpMesh->mNumFaces; ++j) {
            // ibo was causing loading errors due to some improperly formatted obj and dae 
            // files I'm using as assets.
            // commenting out for now.
            /*auto faceI = assimpMesh->mFaces[i];
            if (faceI.mNumIndices < 3) {
                continue;
            }
            assert(faceI.mNumIndices == 3);
            
            indices.push_back(assimpMesh->mFaces[i].mIndices[0]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[1]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[2]);*/
        }

        drawable_components_.push_back(std::shared_ptr<Drawable>(new Drawable(&vertices, &indices, GL_TRIANGLES)));
    }

    // this also caused issues - commenting out for now
    /*std::vector<GLuint> m_Textures_ = NULL;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = scene->mMaterials[i];

      
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                GLuint tex_id = -1;
                ShaderProgram::LoadTextureFromFile(path.C_Str(), tex_id, true);
                m_Textures[i].push_back(tex_id);
            }
        }
    }*/
}

void SceneObject::AssimpLoadCollada(const char* file_path) {
    AssimpLoadObj(file_path);
}

#ifdef BONE
void SceneObject::AssimpLoadRiggedCollada(const char* file_path) {
    // TODO - MODIFY ME TO ACTUALLY HANDLE THE RIGGING


    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file_path,
        aiProcess_GenSmoothNormals |
        aiProcess_GenUVCoords |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        //aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);
    if ((!scene) || (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
        ErrorHandler::ThrowError("AssimpLoadObj error loading file: " + std::string(importer.GetErrorString()));
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* assimpMesh = scene->mMeshes[i];

        std::vector<Vertex> vertices = std::vector<Vertex>();
        std::vector<int> indices = std::vector<int>();

        for (unsigned int j = 0; j < assimpMesh->mNumVertices; ++j) {
            vertices.push_back(Vertex(
                glm::vec3(assimpMesh->mVertices[j].x, assimpMesh->mVertices[j].y, assimpMesh->mVertices[j].z),
                glm::vec3(assimpMesh->mNormals[j].x, assimpMesh->mNormals[j].y, assimpMesh->mNormals[j].z),
                glm::vec2(assimpMesh->mTextureCoords[0][j].x, assimpMesh->mTextureCoords[0][j].y)
            ));
        }

        
        for (uint i = 0; i < assimpMesh->mNumBones; ++i) {
            uint BoneIndex = 0;
            string BoneName(assimpMesh->mBones[i]->mName.data);

            if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
                BoneIndex = m_NumBones;
                m_NumBones++;
                BoneInfo bi;
                m_BoneInfo.push_back(bi);
            }
            else {
                BoneIndex = m_BoneMapping[BoneName];
            }

            m_BoneMapping[BoneName] = BoneIndex;
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

            for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
                uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
                float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
                Bones[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }

        for (unsigned int j = 0; j < assimpMesh->mNumFaces; ++j) {
            // ibo was causing loading errors due to some improperly formatted obj and dae 
            // files I'm using as assets.
            // commenting out for now.
            /*auto faceI = assimpMesh->mFaces[i];
            if (faceI.mNumIndices < 3) {
                continue;
            }
            assert(faceI.mNumIndices == 3);

            indices.push_back(assimpMesh->mFaces[i].mIndices[0]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[1]);
            indices.push_back(assimpMesh->mFaces[i].mIndices[2]);*/
        }

        drawable_components_.push_back(std::shared_ptr<Drawable>(new Drawable(&vertices, &indices, GL_TRIANGLES)));
    }

    /*std::vector<GLuint> m_Textures_ = NULL;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = scene->mMaterials[i];


        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                GLuint tex_id = -1;
                ShaderProgram::LoadTextureFromFile(path.C_Str(), tex_id, true);
                m_Textures[i].push_back(tex_id);
            }
        }
    }*/
}
#endif BONE

void SceneObject::Load(const char* file_path, Filetype mesh_file_type) {
    drawable_components_.clear();

    if (mesh_file_type == Filetype::OBJ) {
        AssimpLoadObj(file_path);
    } else if (mesh_file_type == Filetype::COLLADA) {
        AssimpLoadCollada(file_path);
    }
}

/***********************************************************/
/********* POSITION AND TRANSFORMATION INFORMATION *********/
/***********************************************************/

glm::vec3 SceneObject::GetGlobalPosition() {
    if (multiple_transforms_.size() > 0) {
        return glm::vec3(0.f);
    }

    return glm::vec3(global_transform_[3]);
}

glm::mat4 SceneObject::GetGlobalTransf_NoTranslation() {
    if (multiple_transforms_.size() > 0) {
        return glm::mat4(1.f);
    }
    
    glm::mat4 returning = global_transform_;
    returning[3][0] = 0;
    returning[3][1] = 0;
    returning[3][2] = 0;
    return returning;
}

glm::mat4 SceneObject::GetGlobalTransform() {
    if (multiple_transforms_.size() > 0) {
        return glm::mat4(1.f);
    }

    return global_transform_;
}

void SceneObject::SetGlobalTransform(const glm::mat4& transf) {
    if (multiple_transforms_.size() > 0) {
        return;
    }

    global_transform_ = transf;
}

void SceneObject::AddGlobalTransform(const glm::mat4& transf) {
    multiple_transforms_.push_back(transf);
}

/***********************************************************/
/****************** DRAWING INFORMATION ********************/
/***********************************************************/

void SceneObject::SetTexture(GLuint tex_id) {
    for (std::shared_ptr<Drawable> d : this->drawable_components_) {
        d->SetHandleLocation(HandleType::TEX, tex_id);
    }
}

void SceneObject::SetTextures(GLuint tex_id1, GLuint tex_id2_opacity) {
    for (std::shared_ptr<Drawable> d : this->drawable_components_) {
        d->SetHandleLocation(HandleType::TEX, tex_id1);
        d->SetHandleLocation(HandleType::TEX2, tex_id2_opacity);
    }
}

void SceneObject::SetDrawMode(const GLenum& e) {
    for (std::shared_ptr<Drawable> d : this->drawable_components_) {
        d->SetDrawMode(e);
    }
}

void SceneObject::CreateSelf() {
    if (!program_) {
        return;
    }
    for (std::shared_ptr<Drawable> d : drawable_components_) {
        //GLuint texture_handle = -1; // NEED BETTER WAY TO HANDLE TEXTURE
        program_ptr_->CreateDrawable(d, d->GetHandleLocation(HandleType::TEX)); //----------------------this is where issue is.
    }
}

void SceneObject::Draw(const glm::mat4 view_proj) {
    if (multiple_transforms_.size() == 0) {
        for (std::shared_ptr<Drawable> d : drawable_components_) {
            program_ptr_->Draw(d, global_transform_, view_proj);
        }

        for (std::shared_ptr<SceneObject> s : attached_components_) {
            s->Draw(view_proj * global_transform_);
        }

        return;
    }

    for (glm::mat4 m : multiple_transforms_) {
        for (std::shared_ptr<Drawable> d : drawable_components_) {
            program_ptr_->Draw(d, m, view_proj);
        }

        for (std::shared_ptr<SceneObject> s : attached_components_) {
            s->Draw(view_proj * m);
        }
    }
}

glm::vec3 SceneObject::MoveInDirection(std::shared_ptr<Camera>& camera, const glm::vec3& offset) {
    glm::mat4 transformation = global_transform_;
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transformation, scale, rotation, translation, skew, perspective);
    rotation = glm::conjugate(rotation);
    glm::mat4 rot = glm::toMat4(rotation);

    global_transform_ = rot * glm::translate(glm::mat4(1.f), offset) * glm::inverse(rot) * global_transform_;

    if (camera != nullptr) {
        camera->ref = glm::vec3(global_transform_ * glm::vec4(0, 0, 0, 1));
        camera->eye = glm::vec3(
            rot * glm::translate(glm::mat4(1.f), offset) * glm::inverse(rot) * glm::vec4(camera->eye, 1)
        );

        camera->RecomputeAttributes();
    }

    return glm::vec3(global_transform_ * glm::vec4(0, 0, 0, 1));
}

void SceneObject::TurnInDirection(bool positive) {
    const float TURN_DEG = float(M_PI / 12.f);
    global_transform_ = glm::rotate(global_transform_, positive ? TURN_DEG : -TURN_DEG, glm::vec3(0, 1, 0));
}



