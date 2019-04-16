#include "simple_program.h"

SimpleProgram::SimpleProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
    using_handles_.insert(HandleType::TEX);
}
SimpleProgram::~SimpleProgram() {}

void SimpleProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
    ErrorHandler::PrintGLErrorLog();
    UseMe();
    ErrorHandler::PrintGLErrorLog();

    vector<unsigned int>& ibo_data = d->indices();
    vector<Vertex>& data = d->vertices();
    vector<GLfloat> temp_data = {};

    glm::vec3 col_temp(1.f);
    for (int i = 0; i < ibo_data.size(); ++i) {
        temp_data.push_back(data[ibo_data[i]].pos.x);
        temp_data.push_back(data[ibo_data[i]].pos.y);
        temp_data.push_back(data[ibo_data[i]].pos.z);
        temp_data.push_back(col_temp.r); //data[ibo_data[i]].col.r);
        temp_data.push_back(col_temp.g); //data[ibo_data[i]].col.g);
        temp_data.push_back(col_temp.b); //data[ibo_data[i]].col.b);
        temp_data.push_back(data[ibo_data[i]].uv.x);
        temp_data.push_back(data[ibo_data[i]].uv.y);
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

void SimpleProgram::BeforeDraw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const WindowMaintainer* m) {
    GLuint vbo;
    GLuint vao;
    d->GetHandleLocation(HandleType::VBO, &vbo);
    d->GetHandleLocation(HandleType::VAO, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);

    for (unsigned int i = 0; i < num_attributes; ++i) {
        glEnableVertexAttribArray(i);
    }

    SetUniformMat4(global_transform, GLuint(0));

    if (d->UsingHandle(HandleType::TEX) != 0) {
        GLuint tex;
        d->GetHandleLocation(HandleType::TEX, &tex);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
}

void SimpleProgram::AfterDraw(std::shared_ptr<Drawable>& d) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (unsigned int i = 0; i < num_attributes; ++i) {
        glDisableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}