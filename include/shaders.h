#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <string>

std::string LoadShaderCodeFromFile(std::string filePath);
GLuint CompileShader(GLenum shaderType, std::string shaderPath);
GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath);


#endif // SHADERS_H