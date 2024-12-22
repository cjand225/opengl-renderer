#ifndef SHADERS_H
#define SHADERS_H

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include <string>
#include <vector>

std::string LoadShaderCodeFromFile(std::string filePath);
GLuint      CompileShader(GLenum shaderType, std::string shaderPath);
GLuint      LinkShaders(const std::vector<GLuint>& shaderIDs);
GLuint      LoadShaders(const std::vector<std::pair<GLenum, std::string>>& shaderInfo);

#endif  // SHADERS_H
