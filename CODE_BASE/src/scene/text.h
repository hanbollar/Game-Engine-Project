
#pragma once

#include <map>

#include "globals.h"
#include "typedefs.h"
#include "drawable.h"

class Text : public Drawable {
private:

protected:
    glm::ivec2 xy_;
    int size_;

    const char* name_;

public:
    Text(const char* name);
    Text(vector<glm::vec2>* pos, vector<glm::vec2>* uvs, const char* name);
    Text(glm::ivec2& xy, int& size, const char* name);
	virtual ~Text();

    vector<glm::vec2> pos_uvs_;
    vector<glm::vec2> pos_vertices_;

    void writeToText2D(const char* text);
};