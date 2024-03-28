#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <string>

GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath);

#endif // SHADERS_H