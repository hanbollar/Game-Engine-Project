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

    vector<unsigned short> indices = d->indices();
    vector<Vertex> data = d->vertices();

    vector<glm::vec3> temp_pos;
    for (int i = 0; i < data.size(); ++i) {
        temp_pos.push_back(glm::vec3(data[i].pos));
    }

    GLuint vbo = -1;
    GLuint vao = -1;
    GLuint ebo = -1; //------------

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLsizei count = temp_pos.size(); //---- temp_pos.size();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), temp_pos.data(), GL_STATIC_DRAW);

    //----------------
    //glGenBuffers(1, &ebo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);

    //---
    //d->SetHandleLocation(HandleType::EBO, ebo);

    d->SetElementCount(count);
}

void DefaultProgram::Draw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj) {
    /// BEFORE DRAW
    GLuint vbo = d->GetHandleLocation(HandleType::VBO);
    GLuint vao = d->GetHandleLocation(HandleType::VAO);

    //--
    //GLuint ebo = d->GetHandleLocation(HandleType::EBO);

    glUseProgram(program_);
   

    glBindVertexArray(vao);

    ShaderProgram::SetUniformMat4(global_transform, GLuint(0));
    ShaderProgram::SetUniformMat4(view_proj, GLuint(1));
    if (timer_) {
        ShaderProgram::SetUniformFloat(timer_->GetTime(), GLuint(2));
    }
    if (resolution_.x != -1 && resolution_.y != -1) {
        ShaderProgram::SetUniformVec2(resolution_, GLuint(3));
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //---
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 


    /// DURING DRAW
   glDrawArrays(GL_TRIANGLE_STRIP, 0, d->ElementCount());
    /*if (d->indices().size() == 0) {
        std::cout << "here" << std::endl;
    }
    glDrawElements(
        GL_TRIANGLES,      // mode
        d->indices().size(),    // count
        GL_UNSIGNED_SHORT, // type
        (void*)0           // element array buffer offset
    );*/

    /// AFTER DRAW
    glDisableVertexAttribArray(0);
}