#include "default_program.h"

DefaultProgram::DefaultProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
    using_handles_.insert(HandleType::TEX);
    using_handles_.insert(HandleType::EBO);
}
DefaultProgram::~DefaultProgram() {}

void DefaultProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
    ErrorHandler::PrintGLErrorLog();
    UseMe();
    ErrorHandler::PrintGLErrorLog();

    vector<GLuint> indices = d->indices();
    vector<Vertex> data = d->vertices();

    vector<glm::vec3> temp_pos;
    for (int i = 0; i < indices.size(); ++i) {
        temp_pos.push_back(glm::vec3(data[indices[i]].pos));
    }

    /*vector<glm::vec3> indexed_vertices;
    vector<glm::vec2> indexed_uvs;
    vector<glm::vec3> indexed_normals;
    vector<glm::vec3> indexed_colors;
    for (int i = 0; i < indices.size(); ++i) {
        indexed_vertices.push_back(data[i].pos);
        indexed_uvs.push_back(data[i].uv);
        indexed_colors.push_back(data[i].col);
        indexed_normals.push_back(data[i].nor);
    }

    ErrorHandler::PrintGLErrorLog();

    d->SetElementCount(static_cast<GLsizei>(indices.size()));// temp_data.size()));

    GLuint vbo = -1;
    GLuint vao = -1;
    GLuint ebo = -1;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);

    // VAO - sub elements in each Vertex
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

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
    glBindVertexArray(0);

    ErrorHandler::PrintGLErrorLog();

    num_attributes = 3;

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);

    if (texture_handle != -1) {
        d->SetHandleLocation(HandleType::TEX, texture_handle);
    }

    ErrorHandler::PrintGLErrorLog();*/

    GLuint vbo = -1;
    GLuint vao = -1;
    GLuint ebo = -1;

    /*glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    /*glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));*/

    //glBindVertexArray(0);

    //num_attributes = 1;
    //d->SetElementCount(static_cast<GLsizei>(indices.size()));

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(temp_pos), temp_pos.data(), GL_STATIC_DRAW);

    d->SetHandleLocation(HandleType::VBO, vbo);
    d->SetHandleLocation(HandleType::VAO, vao);
    d->SetHandleLocation(HandleType::EBO, ebo);
}

void DefaultProgram::BeforeDraw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const WindowMaintainer* m) {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    d->GetHandleLocation(HandleType::VBO, &vbo);
    d->GetHandleLocation(HandleType::VAO, &vao);
    d->GetHandleLocation(HandleType::EBO, &ebo);

    ErrorHandler::PrintGLErrorLog();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    ErrorHandler::PrintGLErrorLog();
    // DRAW
    glDrawArrays(GL_TRIANGLES, 0, d->indices().size()); // 3 indices starting at 0 -> 1 triangle

    ErrorHandler::PrintGLErrorLog();
    // VAO
    /*glBindVertexArray(vao);
    for (unsigned int i = 0; i < num_attributes; ++i) {
        glEnableVertexAttribArray(i);
    }
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    // nor
    /*glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));
    // tex
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));*/

   // SetUniformMat4(global_transform, GLuint(0));
    //SetUniformMat4(m->ViewProjection(), GLuint(1));
    
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
    /*glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (unsigned int i = 0; i < num_attributes; ++i) {
        glDisableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);*/


    

    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

//    glDisableVertexAttribArray(0);

}