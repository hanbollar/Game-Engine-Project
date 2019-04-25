#include "text_handler.h"

const char* uv_map_file_path = "resources/TEXT/uvmap.DDS";
const char* font_map_file_path = "resources/TEXT/berlin.DDS";

const char* screen_text_vertexshader_file = "shader_files/screenText.vertexshader";
const char* screen_text_fragshader_file = "shader_files/screenText.fragmentshader";

TextHandler::TextHandler()
    :   SceneObject(),
        tex_font_map_(-1),
        text_data_program_(-1)
{}

TextHandler::~TextHandler() {
    glDeleteProgram(text_data_program_);
    glDeleteTextures(1, &tex_font_map_);
}

void TextHandler::initText2D(int x, int y, int size, const char* name, const TextId id) {
    if (tex_font_map_ == -1 || text_data_program_ == -1) {
        // preventing missing context issue (instead of having this in TextHandler's constructor)
        tex_font_map_ =
            ShaderProgram::LoadDDSTextureFromFile(font_map_file_path);
        text_data_program_ =
            ShaderProgram::LoadShaderProgramFiles(screen_text_vertexshader_file, screen_text_fragshader_file);
    }

    GLuint vbo = -1;
    GLuint uvb = -1;
    GLuint tex = tex_font_map_;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &uvb);

    std::shared_ptr<Text> text_vis = std::shared_ptr<Text>(new Text(glm::ivec2(x, y), size, name));
    
    text_vis->SetHandleLocation(HandleType::VBO, vbo);
    text_vis->SetHandleLocation(HandleType::UVS, uvb);
    text_vis->SetHandleLocation(HandleType::TEX, tex);

    text_drawable_components_[id] = text_vis;
}

void TextHandler::writeToText2D(const string& text, const TextId id) {
    writeToText2D(text.c_str(), id);
}

void TextHandler::writeToText2D(const char* text, const TextId id) {
    try {
        std::shared_ptr<Text> text_vis = text_drawable_components_.at(id);
        text_vis->writeToText2D(text);
    } catch (std::out_of_range) {
        ErrorHandler::ThrowError("That text field has not been created yet");
    }
}

void TextHandler::Draw() {
    // Draw all current text components on the screen

    std::map<TextId, std::shared_ptr<Text>>::iterator it = text_drawable_components_.begin();
    for (; it != text_drawable_components_.end(); ++it) {
        std::shared_ptr<Text> text_field = it->second;

        GLuint vbo = text_field->GetHandleLocation(HandleType::VBO);
        GLuint uvb = text_field->GetHandleLocation(HandleType::UVS);
        GLuint tex = text_field->GetHandleLocation(HandleType::TEX);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, text_field->pos_vertices_.size() * sizeof(glm::vec2),
            text_field->pos_vertices_.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glBufferData(GL_ARRAY_BUFFER, text_field->pos_uvs_.size() * sizeof(glm::vec2),
            text_field->pos_uvs_.data(), GL_STATIC_DRAW);

        // Bind shader
        glUseProgram(text_data_program_);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        //glUniform1i(Text2DUniformID, 0);
        int texture_unit = 0;
        GLuint texture_sampler_uniform_handle = 2;
        ShaderProgram::SetUniformInt(texture_unit, texture_sampler_uniform_handle);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvb);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Drawing Attributes while allowing for alpha reading
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, text_field->ElementCount());
        glDisable(GL_BLEND);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}