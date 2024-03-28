#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <string>

const char* LoadShaderCodeFromFile(std::string filePath);
GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath);


#endif // SHADERS_H