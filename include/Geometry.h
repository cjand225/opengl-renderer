#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>

#include <string>
#include <vector>

struct Vertex {
    float x, y, z;
};

struct Face {
    std::string      materialName;
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;
};

struct UV {
    float u, v, w;
};

std::vector<Vertex>       flattenVertices(const std::vector<Face>& faces, const std::vector<Vertex>& vertices);
std::vector<UV>           flattenUVs(const std::vector<Face>& faces, const std::vector<UV>& uvs);
std::vector<unsigned int> flattenIndices(const std::vector<Face>& faces);
void                      flattenGroupData(const std::vector<Face>&   faces,
                                           const std::vector<Vertex>& vertices,
                                           const std::vector<UV>&     uvs,
                                           std::vector<Vertex>&       outVertices,
                                           std::vector<UV>&           outUVs,
                                           std::vector<unsigned int>& outIndices);

#endif  // GEOMETRY_H
