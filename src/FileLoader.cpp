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

std::vector<UV> flattenUVs(OBJData& meshData) {
    std::vector<UV> uvs;
    for (const Face& face : meshData.faces) {
        for (int index : face.vertexIndices) {
            if (index < meshData.UVs.size()) {
                uvs.push_back(meshData.UVs[index]);
            } else {
                UV defaultUV = {0.0f, 0.0f, 0.0f};
                uvs.push_back(defaultUV);
            }
        }
    }
    return uvs;
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
    std::cout << "Loading texture: " << filename << std::endl;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 0;
    }

    // Read magic number
    GLuint magicNumber;
    file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    std::cout << "Magic number: " << std::hex << magicNumber << " (should be " << DDS_MAGIC << ")" << std::dec << std::endl;

    if (magicNumber != DDS_MAGIC) {
        std::cerr << "Not a DDS file: " << filename << std::endl;
        file.close();
        return 0;
    }

    // Read header
    DDS_HEADER header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    std::cout << "Texture dimensions: " << header.width << "x" << header.height << std::endl;
    std::cout << "Format flags: " << std::hex << header.ddspf.flags << std::dec << std::endl;
    std::cout << "FourCC: " << std::hex << header.ddspf.fourCC << std::dec << std::endl;

    // Determine format and block size
    GLuint format;
    GLuint blockSize;
    switch (header.ddspf.fourCC) {
        case FOURCC_DXT1:
            format    = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            blockSize = 8;
            std::cout << "Format: DXT1" << std::endl;
            break;
        case FOURCC_DXT3:
            format    = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            blockSize = 16;
            std::cout << "Format: DXT3" << std::endl;
            break;
        case FOURCC_DXT5:
            format    = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            blockSize = 16;
            std::cout << "Format: DXT5" << std::endl;
            break;
        default:
            // Try to handle uncompressed textures
            if (header.ddspf.RGBBitCount == 32 &&
                header.ddspf.RBitMask == 0x000000ff &&
                header.ddspf.GBitMask == 0x0000ff00 &&
                header.ddspf.BBitMask == 0x00ff0000 &&
                header.ddspf.ABitMask == 0xff000000) {
                format    = GL_RGBA;
                blockSize = 4;
                std::cout << "Format: Uncompressed RGBA" << std::endl;
            } else {
                std::cerr << "Unsupported format. FourCC: " << header.ddspf.fourCC
                          << " RGB bits: " << header.ddspf.RGBBitCount
                          << " Flags: " << header.ddspf.flags << std::endl;
                file.close();
                return 0;
            }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set basic parameters - simplified for debugging
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Load texture data
    unsigned int bufferSize;
    if (format == GL_RGBA) {
        // Uncompressed
        bufferSize = header.width * header.height * blockSize;
    } else {
        // Compressed
        bufferSize = std::max<unsigned int>(header.pitchOrLinearSize,
                                            ((header.width + 3) / 4) * ((header.height + 3) / 4) * blockSize);
    }

    std::vector<unsigned char> buffer(bufferSize);
    file.read(reinterpret_cast<char*>(buffer.data()), bufferSize);

    std::cout << "Loading texture: " << header.width << "x" << header.height
              << " Buffer size: " << bufferSize << std::endl;

    if (format == GL_RGBA) {
        // Uncompressed texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.width, header.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    } else {
        // Compressed texture
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, header.width, header.height,
                               0, bufferSize, buffer.data());
    }

    // Check for errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error while loading texture: " << err << std::endl;
    }

    // Verify texture was loaded
    GLint texWidth, texHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
    std::cout << "OpenGL reports texture dimensions: " << texWidth << "x" << texHeight << std::endl;

    file.close();
    return textureID;
}
