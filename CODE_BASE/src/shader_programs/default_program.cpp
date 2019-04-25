#include "default_program.h"

DefaultProgram::DefaultProgram(const char* vertex_file, const char* fragment_file)
    : ShaderProgram(vertex_file, fragment_file) {

    using_handles_.insert(HandleType::VBO);
    using_handles_.insert(HandleType::VAO);
    using_handles_.insert(HandleType::UVS);
    using_handles_.insert(HandleType::TEX);
}
DefaultProgram::~DefaultProgram() {}

void DefaultProgram::CreateDrawable(shared_ptr<Drawable>& d, GLuint texture_handle) {
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

    GLsizei count = temp_pos.size();

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

GLuint DefaultProgram::BeforeRenderToTexture() {
    GLuint fbo = -1;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint tex_rendered;
    glGenTextures(1, &tex_rendered);
    glBindTexture(GL_TEXTURE_2D, tex_rendered);

    // set up our texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_rendered, 0);

    // Set the list of draw buffers.
    const GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    return tex_rendered;
}

void DefaultProgram::AfterRenderToTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void DefaultProgram::Draw(shared_ptr<Drawable> d, const glm::mat4& global_transform, const glm::mat4& view_proj) {    
    /// BEFORE DRAW
    GLuint vbo = d->GetHandleLocation(HandleType::VBO);
    GLuint vao = d->GetHandleLocation(HandleType::VAO);
    GLuint uvb = d->GetHandleLocation(HandleType::UVS);

   // GLuint tex_rendered = -1;

    /*if (d->UsingHandle(HandleType::FBO)) {
        tex_rendered = BeforeRenderToTexture();
    }*/

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

    bool allow_opacity = false;

    if (d->UsingHandle(HandleType::TEX)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, d->GetHandleLocation(HandleType::TEX));
        ShaderProgram::SetUniformInt(0, GLuint(2));

        if (d->UsingHandle(HandleType::TEX2)) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, d->GetHandleLocation(HandleType::TEX2));
            ShaderProgram::SetUniformInt(0, GLuint(3));

            allow_opacity = true;
        }

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    /// DURING DRAW
    if (allow_opacity) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, d->ElementCount());
        glDisable(GL_BLEND);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, d->ElementCount());
    }
    

    /// AFTER DRAW
    glDisableVertexAttribArray(0);

    /*if (d->UsingHandle(HandleType::FBO)) {
        AfterRenderToTexture();
    }*/
}