#include "shaders.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string LoadShaderCodeFromFile(std::string filePath)
{
    std::string shaderCode;
    std::ifstream shaderStream(filePath.c_str(), std::ios::in);

    if(shaderStream.is_open())
    {
        std::stringstream strStream;
        strStream << shaderStream.rdbuf();
        shaderCode = strStream.str();
        shaderStream.close();
    }
    else
    {
        throw std::exception("Impossible to open Vertex Shader.");
    }

    return shaderCode;
}

GLuint CompileShader(GLenum shaderType, std::string shaderPath)
{
    GLuint ShaderID = glCreateShader(shaderType);
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    printf("Compiling Shader: %s\n", shaderPath.c_str());
    std::string shaderCode = LoadShaderCodeFromFile(shaderPath);
    const char* VertexSourcePointer = shaderCode.c_str();
    glShaderSource(ShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(ShaderID);

    // Check Status
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0)
    {
        std::vector<char> ShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
        printf("%s\n", &ShaderErrorMessage[0]);
    }

    return ShaderID;
}

GLuint LinkShaders(const std::vector<GLuint>& shaderIDs)
{
    printf("Linking program\n");
    GLuint programID = glCreateProgram();

    // Attach each shader in the list to the program.
    for (GLuint shaderID : shaderIDs) {
        glAttachShader(programID, shaderID);
    }

    glLinkProgram(programID);

    // Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        std::cerr << "Shader linking error: " << &programErrorMessage[0] << std::endl;
    }

    // Detach and delete all shaders
    for (GLuint shaderID : shaderIDs) {
        glDetachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }

    return programID;
}

GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath)
{
    GLuint vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexFilePath);
    GLuint fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentFilePath);
    std::vector<GLuint> shaders = {vertexShaderID, fragmentShaderID};
    GLuint ProgramID = LinkShaders(shaders);

    // Link Program
    return ProgramID;
}