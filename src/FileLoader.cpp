#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "FileLoader.h"

OBJData loadFromOBJ(std::string& filename) {
    std::ifstream file(filename, std::ifstream::in);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line = "";
    OBJData     data;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string        prefix = "";
        iss >> prefix;

        if (prefix == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            data.vertices.push_back(vertex);
        } else if (prefix == "vt") {
            UV uv;
            iss >> uv.u >> uv.v;

            // Can be optional
            if (!(iss >> uv.w)) {
                uv.w = 0.0f;
            }

            data.UVs.push_back(uv);
        } else if (prefix == "f") {
            Face        face;
            std::string vertexDef;
            while (iss >> vertexDef) {
                std::istringstream vertexStream(vertexDef);
                std::string        vertexIndex;

                std::getline(vertexStream, vertexIndex, '/');

                face.vertexIndices.push_back(std::stoi(vertexIndex) - 1);
            }

            data.faces.push_back(face);
        }
    }

    return data;
}

std::vector<unsigned int> flattenVertices(OBJData& meshData) {
    std::vector<unsigned int> indices;
    for (const Face& face : meshData.faces) {
        for (int index : face.vertexIndices) {
            indices.push_back(static_cast<unsigned int>(index));
        }
    }

    return indices;
}

std::map<std::string, Material> loadMTLFile(std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string                     line;
    std::map<std::string, Material> materials;
    Material                        currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string        prefix;

        iss >> prefix;

        if (prefix == "newmtl") {
            if (!currentMaterial.name.empty()) {
                materials[currentMaterial.name] = currentMaterial;
            }
            currentMaterial = Material();
            iss >> currentMaterial.name;
        } else if (prefix == "Ka") {
            iss >> currentMaterial.ambient.r >> currentMaterial.ambient.g >>
                currentMaterial.ambient.b;
        } else if (prefix == "Kd") {
            iss >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >>
                currentMaterial.diffuse.b;
        } else if (prefix == "Ks") {
            iss >> currentMaterial.specular.r >> currentMaterial.specular.g >>
                currentMaterial.specular.b;
        } else if (prefix == "Ns") {
            iss >> currentMaterial.shininess;
        } else if (prefix == "d" || prefix == "Tr") {
            iss >> currentMaterial.transparency;
        } else if (prefix == "map_Kd") {
            iss >> currentMaterial.diffuseMapPath;
        } else if (prefix == "illum") {
            iss >> currentMaterial.illumination;
        }
    }

    if (!currentMaterial.name.empty()) {
        materials[currentMaterial.name] = currentMaterial;
    }

    return materials;
}

GLuint loadDDSFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    GLuint magicNumber;
    file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    if (magicNumber != DDS_MAGIC) {
        std::cerr << "Not a DDS file: " << filename << std::endl;
        return 0;
    }

    DDS_HEADER header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (header.ddspf.flags != DDPF_FOURCC) {
        std::cerr << "Unsupported Format (Only DXT1, DXT3, DXT5 are supported)"
                  << std::endl;
        return 0;
    }

    GLuint blockSize = (header.ddspf.fourCC == FOURCC_DXT1) ? 8 : 16;
    GLuint format;
    switch (header.ddspf.fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            std::cerr << "Unsupported DXT format" << std::endl;
            return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Read mipmaps
    unsigned int bufferSize = std::max(
        header.pitchOrLinearSize, header.width * header.height * blockSize / 8);
    std::vector<unsigned char> buffer(bufferSize);
    for (GLuint level = 0;
         level < header.mipMapCount && (header.width || header.height); level++) {
        unsigned int size =
            ((header.width + 3) / 4) * ((header.height + 3) / 4) * blockSize;
        file.read(reinterpret_cast<char*>(buffer.data()), size);
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, header.width,
                               header.height, 0, size, buffer.data());

        header.width /= 2;
        header.height /= 2;
    }

    file.close();
    return textureID;
}
