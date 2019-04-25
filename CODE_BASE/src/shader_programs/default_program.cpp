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

void DefaultProgram::Draw(shared_ptr<Drawable>& d, const glm::mat4& global_transform, const glm::mat4& view_proj) {
    /// BEFORE DRAW
    GLuint vbo = d->GetHandleLocation(HandleType::VBO);
    GLuint vao = d->GetHandleLocation(HandleType::VAO);
    GLuint uvb = d->GetHandleLocation(HandleType::UVS);

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

    if (d->UsingHandle(HandleType::TEX)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, d->GetHandleLocation(HandleType::TEX));
        ShaderProgram::SetUniformInt(0, GLuint(2));

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    /// DURING DRAW
   glDrawArrays(GL_TRIANGLES /*GL_TRIANGLE_STRIP*/, 0, d->ElementCount());

    /// AFTER DRAW
    glDisableVertexAttribArray(0);
}

/*
void DefaultProgram::CreateFrameBufferTexture() {
    GLuint fbo = -1;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
    const int width_height = 500;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_height, width_height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ErrorHandler::ThrowError("Unexpected Configuration");
    }

    // The fullscreen quad's FBO
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    // Create and compile our GLSL program from the shaders
    GLuint quad_programID = LoadShaders("Passthrough.vertexshader", "WobblyTexture.fragmentshader");
    GLuint texID = glGetUniformLocation(quad_programID, "renderedTexture");
    GLuint timeID = glGetUniformLocation(quad_programID, "time");
}

void DefaultProgram::RenderToTexture() {

}*/