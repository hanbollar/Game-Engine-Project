#include "shader_program.h"

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

ShaderProgram::ShaderProgram(const char* vertex_file, const char* fragment_file)
    : using_handles_(set<HandleType>()) {

    program_ = LoadShaderProgramFiles(vertex_file, fragment_file);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}

/*********************/
/****** Setters ******/
/*********************/

void ShaderProgram::SetTimer(shared_ptr<Timer> timer) {
    timer_ = timer;
}

void ShaderProgram::SetResolution(glm::vec2& resolution) {
    resolution_ = resolution;
}

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

void ShaderProgram::SetUniformVec2(const glm::vec2& vector2, const GLuint& handle) {
    //UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    }
    else {
        glUniform2f(handle, vector2[0], vector2[1]);
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
        } else {
            ErrorHandler::ThrowError(
                "Impossible to open " + std::string(file_path) + ". Are you in the right directory ?");
        }
    }
    ErrorHandler::ShowError("Compiling shader : " + string(file_path));
    char const * source_pointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &source_pointer, NULL);
    glCompileShader(shader_id);
    ErrorHandler::PrintShaderInfoLog(shader_id);

    return shader_id;
}

GLuint ShaderProgram::LoadTextureFromFile(const char* file_path) {
    if (file_path == nullptr) {
        ErrorHandler::ThrowError("Invalid texture file input!");
    }
    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    ErrorHandler::PrintGLErrorLog();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_path, &width, &height, &nrChannels, 0);
    if (!data) {
        ErrorHandler::ThrowError("Texture not Loaded!");
    }

    // not using RGBA - bc not checking for jpg vs png vs etc file types
    // switched to try catch so can allow for alpha loading if needed
    //if ()
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //} catch (...) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //}

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    ErrorHandler::PrintGLErrorLog();
    return texture_id;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint ShaderProgram::LoadDDSTextureFromFile(const char* file_path) {
    // used as a backup for DDS file loading instead of generic LoadTextureFromFile
    // might use this function - depends on if generic works - tbd

    // below code from: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-11-2d-text/

    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(file_path, "rb");
    if (fp == NULL) {
        printf("%s could not be opened. Are you in the right directory ?\n", file_path); getchar();
        return 0;
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if (width < 1) width = 1;
        if (height < 1) height = 1;
    }

    free(buffer);

    return textureID;
}

