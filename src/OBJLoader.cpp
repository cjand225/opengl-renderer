#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

OBJData loadFromOBJ(std::string filename)
{
    std::ifstream inputFile(filename, std::ifstream::in);

    if (!inputFile.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line = "";
    OBJData data;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string prefix = "";
        iss >> prefix;

        if (prefix == "v")
        {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            data.vertices.push_back(vertex);
        }
        else if (prefix == "f")
        {
            Face face;
            std::string vertexDef;
            while (iss >> vertexDef)
            {
                std::istringstream vertexStream(vertexDef);
                std::string vertexIndex;

                std::getline(vertexStream, vertexIndex, '/');

                face.vertexIndices.push_back(std::stoi(vertexIndex) - 1);
            }

            data.faces.push_back(face);
        }
    }

    return data;
}

std::vector<unsigned int> flattenVertices(OBJData meshData)
{
    std::vector<unsigned int> indices;
    for (const Face& face : meshData.faces) {
        for (int index : face.vertexIndices) {
            indices.push_back(static_cast<unsigned int>(index));
        }
    }

    return indices;
}

