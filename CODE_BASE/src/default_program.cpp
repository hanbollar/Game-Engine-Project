#include "default_program.h"

DefaultProgram::DefaultProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
    using_handles_.insert(HandleType::TEX);
}
DefaultProgram::~DefaultProgram() {}

void DefaultProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
    UseMe();
    ErrorHandler::PrintGLErrorLog();

    vector<unsigned int> ibo_data = d->indices();
    vector<glm::vec3> data = d->positions();
    vector<glm::vec2> uv_data = d->uvs();

    vector<GLfloat> temp_data = {};
    glm::vec3 col(0.f);
    for (int i = 0; i < ibo_data.size(); ++i) {
        temp_data.push_back(data[ibo_data[i]].x);
        temp_data.push_back(data[ibo_data[i]].y);
        temp_data.push_back(data[ibo_data[i]].z);
        temp_data.push_back(col.r);
        temp_data.push_back(col.g);
        temp_data.push_back(col.b);
        temp_data.push_back(uv_data[ibo_data[i]].x);
        temp_data.push_back(uv_data[ibo_data[i]].y);

        if ((i + 1) % 3 == 0) {
            col = glm::vec3(rand() % 255, rand() % 255, rand() % 255);
            col /= 255.f;
        }
    }

    d->SetElementCount(static_cast<unsigned int>(temp_data.size()));

    GLuint vbo = 0;
    GLuint vao = 0;

    GLsizei step_size = 8 * sizeof(GLfloat);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, temp_data.size() * sizeof(GLfloat), temp_data.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step_size, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, step_size, (const GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, step_size, (const GLvoid*)(6 * sizeof(GLfloat)));
    num_attributes = 3;

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);

    if (texture_handle != -1) {
        d->SetHandleLocation(HandleType::TEX, texture_handle);
    }

    ErrorHandler::PrintGLErrorLog();
}

void DefaultProgram::BeforeDraw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, WindowMaintainer* m) {
    GLuint vbo;
    GLuint vao;
    d->GetHandleLocation(HandleType::VBO, &vbo);
    d->GetHandleLocation(HandleType::VAO, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);

    for (unsigned int i = 0; i < num_attributes; ++i) {
        glEnableVertexAttribArray(i);
    }

    SetUniformMat4(global_transform * d->model_matrix(), GLuint(0));
    SetUniformMat4(m->ViewProjection(), GLuint(1));
    
    if (d->UsingHandle(HandleType::TEX)) {
        SetUniformSampler(0, GLuint(2));
        SetUniformFloat(1.f, GLuint(3));

        GLuint tex;
        d->GetHandleLocation(HandleType::TEX, &tex);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    } else {
        //SetUniformFloat(0.f, GLuint(3));
        // TODO: once have texture loading from bmp properly change this back to 0.f
        SetUniformFloat(1.f, GLuint(3));
    }    
}

void DefaultProgram::AfterDraw(std::shared_ptr<Drawable>& d) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (unsigned int i = 0; i < num_attributes; ++i) {
        glDisableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}