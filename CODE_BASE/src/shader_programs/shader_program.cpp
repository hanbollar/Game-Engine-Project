#include "shader_program.h"

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

ShaderProgram::ShaderProgram(const char* vertex_file,
    const char* fragment_file)
    : using_handles_(set<HandleType>()) {

    program_ = LoadShaderProgramFiles(vertex_file, fragment_file);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}

/*********************/
/****** Setters ******/
/*********************/


void ShaderProgram::SetUniformDouble(const float& val, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1d(handle, val);
    }
}

void ShaderProgram::SetUniformFloat(const float& val, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1f(handle, val);
    }
}

void ShaderProgram::SetUniformInt(const int& val, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1i(handle, val);
    }
}

void ShaderProgram::SetUniformVec3(const glm::vec3 &vector3, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform3f(handle, vector3[0], vector3[1], vector3[2]);
    }
}

void ShaderProgram::SetUniformVec4(const glm::vec4 &vector4, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform4f(handle, vector4[0], vector4[1], vector4[2], vector4[3]);
    }
}

void ShaderProgram::SetUniformMat4(const glm::mat4 &matrix, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniformMatrix4fv(handle, 1, GL_FALSE, &matrix[0][0]);
    }
}

void ShaderProgram::SetUniformSampler(const int& val, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    }
    else {
        glUniform1i(handle, val);
    }
}

/************************************/
/****** Static Loading Methods ******/
/************************************/

GLuint ShaderProgram::LoadShaderProgramFiles(const char * vertex_file_path, const char * fragment_file_path) {
    GLuint vertex_shader_id = LoadShaderFile(vertex_file_path, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = LoadShaderFile(fragment_file_path, GL_FRAGMENT_SHADER);

    cout << "Linking program" << endl;;
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    ErrorHandler::PrintLinkInfoLog(program_id);

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

GLuint ShaderProgram::LoadShaderFile(const char* file_path, GLenum shader_type) {
    GLuint shader_id = glCreateShader(shader_type);
    std::string shader_code;
    {
        std::ifstream shader_stream(file_path, std::ios::in);
        if (shader_stream.is_open()) {
            std::stringstream sstr;
            sstr << shader_stream.rdbuf();
            shader_code = sstr.str();
            shader_stream.close();
        }
        else {
            cout << "Impossible to open " << file_path
                << "Are you in the right directory ?" << endl;
            throw;
        }
    }
    cout << "Compiling shader : " << file_path << endl;;
    char const * source_pointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &source_pointer, NULL);
    glCompileShader(shader_id);
    ErrorHandler::PrintShaderInfoLog(shader_id);

    return shader_id;
}

/*
GLuint ShaderProgram::LoadGenericTexture(const GLsizei& width, const GLsizei& height, unsigned char* data) {
    if (data == nullptr) {
        return -1;
    }
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    ErrorHandler::PrintGLErrorLog();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    ErrorHandler::PrintGLErrorLog();

    return texture_id;
}*/

void ShaderProgram::LoadTextureFromFile(const char* file_path, GLuint& tex_id, bool createNewId) {
    if (file_path == nullptr) {
        ErrorHandler::ThrowError("Invalid texture file input!");
    }
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    ErrorHandler::PrintGLErrorLog();

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);*/

    ////

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /*
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        m_textureImage->width(), m_textureImage->height(),
        0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_textureImage->bits());
    context->printGLErrorLog();*/

    ////

    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_path, &width, &height, &nrChannels, 0);
    if (!data) {
        ErrorHandler::ThrowError("Texture not Loaded!");
    }

    /*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    ErrorHandler::PrintGLErrorLog();

    tex_id = texture_id;

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    ErrorHandler::PrintGLErrorLog();
}

