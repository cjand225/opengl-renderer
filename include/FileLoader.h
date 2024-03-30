#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <map>

struct Vertex
{
    float x, y, z;
};

struct Face
{
    std::vector<int> vertexIndices;
};

struct OBJData
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

struct Material
{
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float transparency;
    std::string diffuseMapPath;
    int illumination;
};

OBJData loadFromOBJ(std::string& filename);
std::map<std::string, Material> loadMTLFile(std::string& filename);
std::vector<unsigned int> flattenVertices(OBJData& meshData);

#endif // FILELOADER_H
