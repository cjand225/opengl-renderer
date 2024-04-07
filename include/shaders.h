#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <string>
#include <vector>

std::string LoadShaderCodeFromFile(std::string filePath);
GLuint CompileShader(GLenum shaderType, std::string shaderPath);
GLuint LinkShaders(const std::vector<GLuint>& shaderIDs);
GLuint LoadShaders(const std::vector<std::pair<GLenum, std::string>>& shaderInfo);

#endif // SHADERS_H
