#include "simple_program.h"

SimpleProgram::SimpleProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
    using_handles_.insert(HandleType::UVS);
    using_handles_.insert(HandleType::TEX);
}
SimpleProgram::~SimpleProgram() {}

void SimpleProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
    ErrorHandler::PrintGLErrorLog();
    UseMe();

    vector<unsigned short> indices = d->indices();
    vector<Vertex> data = d->vertices();

    vector<glm::vec3> temp_pos;
    vector<glm::vec2> uv_pos;
    for (int i = 0; i < data.size(); ++i) {
        temp_pos.push_back(glm::vec3(data[i].pos));
        uv_pos.push_back(glm::vec2(data[i].uv));
    }

    GLuint vbo = -1;
    GLuint vao = -1;
    GLuint uvb = -1;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLsizei count = static_cast<GLsizei>(temp_pos.size());

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), temp_pos.data(), GL_STATIC_DRAW);

    if (d->UsingHandle(HandleType::TEX)) {
        glGenBuffers(1, &uvb);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glBufferData(GL_ARRAY_BUFFER, uv_pos.size() * sizeof(glm::vec2), uv_pos.data(), GL_STATIC_DRAW);
        d->SetHandleLocation(HandleType::UVS, uvb);
    }

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);

    d->SetElementCount(count);
}

void SimpleProgram::Draw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj) {
    /// BEFORE DRAW
    GLuint vbo = d->GetHandleLocation(HandleType::VBO);
    GLuint vao = d->GetHandleLocation(HandleType::VAO);
    GLuint uvb = d->GetHandleLocation(HandleType::UVS);

    glUseProgram(program_);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if (d->UsingHandle(HandleType::TEX)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, d->GetHandleLocation(HandleType::TEX));
        ShaderProgram::SetUniformInt(0, GLuint(2));

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    /// DURING DRAW
    glDrawArrays(GL_TRIANGLE_STRIP, 0, d->ElementCount());

    /// AFTER DRAW
    glDisableVertexAttribArray(0);
}