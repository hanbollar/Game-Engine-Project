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

    vector<GLuint> ibo_data = d->indices();
    vector<Vertex> data = d->vertices();
    /*vector<GLfloat> temp_data = {};
    for (int i = 0; i < ibo_data.size(); ++i) {
        temp_data.push_back(data[ibo_data[i]].pos.x);
        temp_data.push_back(data[ibo_data[i]].pos.y);
        temp_data.push_back(data[ibo_data[i]].pos.z);
        temp_data.push_back(data[ibo_data[i]].col.r);
        temp_data.push_back(data[ibo_data[i]].col.g);
        temp_data.push_back(data[ibo_data[i]].col.b);
        temp_data.push_back(data[ibo_data[i]].uv.x);
        temp_data.push_back(data[ibo_data[i]].uv.y);
    }*/

    ErrorHandler::PrintGLErrorLog();

    d->SetElementCount(static_cast<GLsizei>(ibo_data.size()));// temp_data.size()));

    GLuint vbo = -1;
    GLuint vao = -1;
    GLuint ebo = -1;

    ErrorHandler::PrintGLErrorLog();

    // VAO - sub elements in each Vertex
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);

    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // col
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    
    // RESET
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibo_data), ibo_data.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    ErrorHandler::PrintGLErrorLog();

    // VBO - general geo
    
    

    ErrorHandler::PrintGLErrorLog();

    // EBO - indices
    
  

    ErrorHandler::PrintGLErrorLog();

    num_attributes = 3;

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);
    d->SetHandleLocation(HandleType::EBO, ebo);

    if (texture_handle != -1) {
        d->SetHandleLocation(HandleType::TEX, texture_handle);
    }

    ErrorHandler::PrintGLErrorLog();
}

void DefaultProgram::BeforeDraw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const WindowMaintainer* m) {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    d->GetHandleLocation(HandleType::VBO, &vbo);
    d->GetHandleLocation(HandleType::VAO, &vao);
    d->GetHandleLocation(HandleType::EBO, &ebo);

    // VAO
    glBindVertexArray(vao);
    // VBO
    /*glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // col
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));*/

    SetUniformMat4(global_transform, GLuint(0));
    SetUniformMat4(m->ViewProjection(), GLuint(1));
    
    /*if (d->UsingHandle(HandleType::TEX)) {
        SetUniformSampler(0, GLuint(2));
        SetUniformFloat(1.f, GLuint(3));

        GLuint tex;
        d->GetHandleLocation(HandleType::TEX, &tex);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
    } else {
        //SetUniformFloat(0.f, GLuint(3));
        SetUniformFloat(1.f, GLuint(3));
    }  */
}

void DefaultProgram::AfterDraw(std::shared_ptr<Drawable>& d) {
    /*glBindBuffer(GL_ARRAY_BUFFER, 0);*/

    /*for (unsigned int i = 0; i < num_attributes; ++i) {
        glDisableVertexAttribArray(i);
    }*/

    glBindVertexArray(0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glUseProgram(0);
}