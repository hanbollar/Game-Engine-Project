#include "primitive.h"

Primitive::Primitive() : type_(ShapeType::CUBE), global_transform_(glm::mat4(1.f)) 
{}

Primitive::Primitive(const glm::mat4& m, const ShapeType& type) : type_(type), global_transform_(m)
{}

Primitive::~Primitive() 
{}

bool Primitive::Within(const glm::vec3& p) {
    glm::vec3 using_p(glm::inverse(global_transform_) * glm::vec4(p, 1.f));

    switch (type_) {
    case ShapeType::SPHERE:
        // unit sphere centered at the origin
        return glm::distance(p, glm::vec3(0.f)) <= 1;
    case ShapeType::CUBE:
        // unit cube centered at the origin
        return p.x >= -.5f && p.x <= .5f
            && p.y >= -.5f && p.y <= .5f
            && p.z >= -.5f && p.z <= .5f;
    case ShapeType::CYLINDER:
        // unit vertical cylinder centered at the origin
        // within height
        // within radius of 1
        return p.y >= -.5f && p.y <= .5f
            && (p.x * p.x + p.z * p.z <= 1.f);
    default:
        ErrorHandler::ThrowError("Unexpected Primitive Shape");
    }
}
