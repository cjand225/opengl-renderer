#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <string>

struct Material {
    std::string name;
    glm::vec3   ambient;
    glm::vec3   diffuse;
    glm::vec3   specular;
    float       shininess;
    float       transparency;
    std::string diffuseMapPath;
    int         illumination;
    GLuint      texture;
};

#endif  // MATERIAL_H
