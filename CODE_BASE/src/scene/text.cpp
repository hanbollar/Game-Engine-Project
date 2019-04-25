#include "text.h"

const int BASE_SIZE = 100;
const float PIXEL_OFFSET = 16.f;
const float INVPIXEL_OFFSET = 1.f / PIXEL_OFFSET;

Text::Text(const char* name)
    : Drawable(),
    pos_uvs_(vector<glm::vec2>()), pos_vertices_(vector<glm::vec2>()),
    name_(name),
    xy_(glm::ivec2(0)), size_(BASE_SIZE)
{}

Text::Text(vector<glm::vec2>* pos, vector<glm::vec2>* uvs, const char* name)
    : Drawable(),
    pos_uvs_(*pos), pos_vertices_(*uvs),
    name_(name),
    xy_(glm::ivec2(0)), size_(BASE_SIZE)
{}

Text::Text(glm::ivec2& xy, int& size, const char* name)
    : Drawable(),
    pos_uvs_(vector<glm::vec2>()), pos_vertices_(vector<glm::vec2>()),
    name_(name),
    xy_(xy), size_(size)
{}

Text::~Text() {
    pos_uvs_.clear();
    pos_vertices_.clear();
}

// Setting this up following opengl-tutorial and learn-opengl.
// modified for my datastructures and configuration to optimize.
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-11-2d-text/

void Text::writeToText2D(const char* text) {
    unsigned int length = strlen(text);

    glm::vec2 up_left(0.f);
    glm::vec2 up_right(0.f);
    glm::vec2 down_right(0.f);
    glm::vec2 down_left(0.f);

    int x = xy_[0];
    int y = xy_[1];

    pos_vertices_.clear();
    pos_uvs_.clear();

    for (unsigned int i = 0; i < length; ++i) {
        // vertices
        up_left = glm::vec2(x + i * size_, y + size_);
        up_right = glm::vec2(x + i * size_ + size_, y + size_);
        down_right = glm::vec2(x + i * size_ + size_, y);
        down_left = glm::vec2(x + i * size_, y);

        pos_vertices_.push_back(up_left);
        pos_vertices_.push_back(down_left);
        pos_vertices_.push_back(up_right);
        pos_vertices_.push_back(down_right);
        pos_vertices_.push_back(up_right);
        pos_vertices_.push_back(down_left);

        // uvs
        char character = text[i];
        float uv_x = (character % int(PIXEL_OFFSET)) / PIXEL_OFFSET;
        float uv_y = (character / int(PIXEL_OFFSET)) / PIXEL_OFFSET;

        up_left = glm::vec2(uv_x, uv_y);
        up_right = glm::vec2(uv_x + INVPIXEL_OFFSET, uv_y);
        down_right = glm::vec2(uv_x + INVPIXEL_OFFSET, uv_y + INVPIXEL_OFFSET);
        down_left = glm::vec2(uv_x, uv_y + INVPIXEL_OFFSET);

        pos_uvs_.push_back(up_left);
        pos_uvs_.push_back(down_left);
        pos_uvs_.push_back(up_right);
        pos_uvs_.push_back(down_right);
        pos_uvs_.push_back(up_right);
        pos_uvs_.push_back(down_left);
    }

    SetElementCount(pos_vertices_.size());
}