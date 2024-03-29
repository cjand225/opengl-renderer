#include "shaders.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

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

GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath)
{
    GLuint VertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexFilePath);
    GLuint FragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentFilePath);

    // Link Program
    GLint Result = GL_FALSE;
    int InfoLogLength;
    printf("Linking Program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check Program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}