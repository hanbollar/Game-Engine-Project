#include "default_program.h"

DefaultProgram::DefaultProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
}
DefaultProgram::~DefaultProgram() {}

void DefaultProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
    ErrorHandler::PrintGLErrorLog();
    UseMe();

    vector<GLuint> indices = d->indices();
    vector<Vertex> data = d->vertices();

    vector<glm::vec3> temp_pos;
    for (int i = 0; i < data.size(); ++i) {
        temp_pos.push_back(glm::vec3(data[i].pos));
    }

    GLuint vbo = -1;
    GLuint vao = -1;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLsizei count = temp_pos.size();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), temp_pos.data(), GL_STATIC_DRAW);

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);

    d->SetElementCount(count);
}

void DefaultProgram::Draw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj) {
    GLuint vbo;
    GLuint vao;
    d->GetHandleLocation(HandleType::VBO, &vbo);
    d->GetHandleLocation(HandleType::VAO, &vao);

    glUseProgram(program_);
    glBindVertexArray(vao);

    ShaderProgram::SetUniformMat4(global_transform, GLuint(0));
    ShaderProgram::SetUniformMat4(view_proj, GLuint(1));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /// during draw
    glDrawArrays(GL_TRIANGLES, 0, d->ElementCount());

    /// after draw
    glDisableVertexAttribArray(0);
}