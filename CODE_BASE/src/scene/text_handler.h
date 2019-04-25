#pragma once

#include <map>

#include "globals.h"
#include "scene/drawable.h"
#include "scene_object.h"
#include "scene/text.h"

class TextHandler : SceneObject {
private:
    std::map<TextId, std::shared_ptr<Text>> text_drawable_components_;

    GLuint tex_uv_map_;
    GLuint tex_font_map_;
    GLuint text_data_program_;
public:
	TextHandler();
	virtual ~TextHandler();

    void initText2D(int x, int y, int size, const char* name, const TextId id);

    void writeToText2D(const string& text, const TextId id);
    void writeToText2D(const char* text, const TextId id);

    virtual void Draw();
};
