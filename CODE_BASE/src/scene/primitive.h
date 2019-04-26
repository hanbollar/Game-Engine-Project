#pragma once

#include "globals.h"
#include "typedefs.h"

class Primitive {
private:
    ShapeType type_;
    glm::mat4 global_transform_;

public:
    Primitive();
    Primitive(const glm::mat4& m, const ShapeType& type);
    ~Primitive();

    inline void SetGlobalTransform(const glm::mat4& m) { global_transform_ = m; }
    inline void SetType(const ShapeType& type) { type_ = type; }

    inline glm::mat4 GetGlobalTransform() { return global_transform_; }

    ShapeType GetType() { return type_; }

    bool Within(const glm::vec3& p);
};
