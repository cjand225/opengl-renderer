#include "shaders.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

GLuint LoadShaders(std::string vertexFilePath, std::string fragmentFilePath)
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderCode;
    std::ifstream VertextShaderStream(vertexFilePath.c_str(), std::ios::in);

    if(VertextShaderStream.is_open())
    {
        std::stringstream strStream;
        strStream << VertextShaderStream.rdbuf();
        VertexShaderCode = strStream.str();
        VertextShaderStream.close();
    }
    else
    {
        std::cerr << "Impossible to open Vertex Shader." << std::endl;
    }

    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragmentFilePath.c_str(), std::ios::in);
    if(FragmentShaderStream.is_open())
    {
        std::stringstream strStream;
        strStream << FragmentShaderStream.rdbuf();
        FragmentShaderCode = strStream.str();
        FragmentShaderStream.close();
    }
    else
    {
        std::cerr << "Impossible to open Fragment Shader." << std::endl;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex
    printf("Compiling Shader: %s\n", vertexFilePath.c_str());
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Status
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment
    printf("Compiling Shader: %s\n", fragmentFilePath.c_str());
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Status
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);


    if(InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link Program
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