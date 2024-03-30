#include "FileLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

OBJData loadFromOBJ(std::string& filename)
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
        else if (prefix == "vt")
        {
            UV uv;
            iss >> uv.u >> uv.v;

            // Can be optional
            if (!(iss >> uv.w)) {
                uv.w = 0.0f;
            }

            data.UVs.push_back(uv);
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

std::vector<unsigned int> flattenVertices(OBJData& meshData)
{
    std::vector<unsigned int> indices;
    for (const Face& face : meshData.faces) {
        for (int index : face.vertexIndices) {
            indices.push_back(static_cast<unsigned int>(index));
        }
    }

    return indices;
}

std::map<std::string, Material> loadMTLFile(std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    std::map<std::string, Material> materials;
    Material currentMaterial;

    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;

        iss >> prefix;

        if (prefix == "newmtl")
        {
            if(!currentMaterial.name.empty())
            {
                materials[currentMaterial.name] = currentMaterial;
            }
            currentMaterial = Material();
            iss >> currentMaterial.name;
        }
        else if (prefix == "Ka") 
        {
            iss >> currentMaterial.ambient.r >> currentMaterial.ambient.g >> currentMaterial.ambient.b;
        }
        else if (prefix == "Kd") 
        {
            iss >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >> currentMaterial.diffuse.b;
        }
        else if (prefix == "Ks") 
        {
            iss >> currentMaterial.specular.r >> currentMaterial.specular.g >> currentMaterial.specular.b;
        }
        else if (prefix == "Ns") 
        {
            iss >> currentMaterial.shininess;
        }
        else if (prefix == "d" || prefix == "Tr") 
        {
            iss >> currentMaterial.transparency;
        }
        else if (prefix == "map_Kd") 
        {
            iss >> currentMaterial.diffuseMapPath;
        }
        else if (prefix == "illum") 
        {
            iss >> currentMaterial.illumination;
        }
    }

    if(!currentMaterial.name.empty())
    {
        materials[currentMaterial.name] = currentMaterial;
    }

    return materials;
}