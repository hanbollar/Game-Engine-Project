#include "shader_program.h"

ShaderProgram::ShaderProgram(const char* vertex_file,
    const char* fragment_file)
    : using_handles_(set<HandleType>()) {

    program_ = LoadShaderProgramFiles(vertex_file, fragment_file);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program_);
}

void ShaderProgram::Draw(std::shared_ptr<Drawable>& d, const glm::mat4& global_transform, WindowMaintainer* m) {
    UseMe();
    BeforeDraw(d, global_transform, m);
    glDrawArrays(d->DrawMode(), 0, d->ElementCount());
    AfterDraw(d);
}

/*********************/
/****** Setters ******/
/*********************/


void ShaderProgram::SetUniformDouble(const float& val, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1d(handle, val);
    }
}

void ShaderProgram::SetUniformFloat(const float& val, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1f(handle, val);
    }
}

void ShaderProgram::SetUniformInt(const int& val, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform1i(handle, val);
    }
}

void ShaderProgram::SetUniformVec3(const glm::vec3 &vector3, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform3f(handle, vector3[0], vector3[1], vector3[2]);
    }
}

void ShaderProgram::SetUniformVec4(const glm::vec4 &vector4, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniform4f(handle, vector4[0], vector4[1], vector4[2], vector4[3]);
    }
}

void ShaderProgram::SetUniformMat4(const glm::mat4 &matrix, const GLuint& handle) {
    UseMe();
    if (handle == -1) {
        ErrorHandler::ThrowError("handle was -1.");
    } else {
        glUniformMatrix4fv(handle, 1, GL_FALSE, &matrix[0][0]);
    }
}

void ShaderProgram::SetUniformSampler(const int& val, const GLuint& handle) {
    UseMe();
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
}

GLuint ShaderProgram::LoadTextureFromBMPFile(const char* file_path) { 
    // open texture data
   /* FILE * file = fopen(file_path, "rb");
    if (!file) {
        cout << "Could not open file: " << file_path << endl;
        throw;
    }

    int width, height;
    short int bpp, planes;
    // get file length
    fseek(file, 18, SEEK_CUR);
    fread(&width, static_cast<size_t>(4), static_cast<size_t>(1), file);
    fread(&height, static_cast<size_t>(4), static_cast<size_t>(1), file);
    long file_length = width * height * 3;

    // allocate buffer
    unsigned char *data = (unsigned char *)malloc(file_length);
    if (!data) {
        ErrorHandler::ThrowError("Memory Error");
        fclose(file);
        return -1;
    }

    // fill pixel data
    fread(&bpp, static_cast<size_t>(2), static_cast<size_t>(1), file);
    fread(&planes, static_cast<size_t>(2), static_cast<size_t>(1), file);
    fseek(file, 24, SEEK_CUR);
    fread(data, file_length, sizeof(unsigned char), file);
    
    char temp;
    for (int i = 0; i < file_length; i += 3) {
        temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
    fclose(file);

    return ShaderProgram::LoadGenericTexture(width, height, data);*/
    
    //--

                                                                  // Open the file
    FILE * file = fopen(file_path, "rb");
    if (!file) {
        ErrorHandler::ThrowError(string("Could not open file: ") + string(file_path));
    }

    char buffer[54]; // TODO: FIX HERE - BUFFER
    if (fread(buffer, 1, 54, file) != 54) {
        ErrorHandler::ThrowError("BMP file formatted wrong. Did not read 54 bytes.\n");
    }

    if (buffer[0] != 'B' || buffer[1] != 'M') {
        ErrorHandler::ThrowError("BMP file formatted wrong. Incorrect header.\n");
    }

    // Read ints from the byte array
    auto dataPos = *(int*)&(buffer[0x0A]);
    auto imageSize = *(int*)&(buffer[0x22]);
    int width = *(int*)&(buffer[0x12]);
    int height = *(int*)&(buffer[0x16]);

    // fix formatting if not filled in
    if (!imageSize) {
        // 3 bytes: rgb
        imageSize = width * height * 3;
    }
    if (!dataPos) {
        // The bmp header
        dataPos = 54;
    }

    // Create a buffer
    unsigned char *data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    return ShaderProgram::LoadGenericTexture(width, height, data);

    // TODO: texture being loaded wrong because error gets thrown
}