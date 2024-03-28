#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>

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

OBJData loadFromOBJ(std::string filename);
std::vector<unsigned int> flattenVertices(OBJData meshData);

#endif // OBJLOADER_H
