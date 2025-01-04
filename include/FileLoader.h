#ifndef FILELOADER_H
#define FILELOADER_H

#ifdef __APPLE__
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>

    // Define DXT compression constants if not available
    #ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
        #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
    #endif
    #ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
        #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
    #endif
    #ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
    #endif
#else
    #include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

struct Vertex {
    float x, y, z;
};

struct Face {
    std::vector<int> vertexIndices;
    std::string      materialName;
};

struct UV {
    float u, v, w;
};

struct OBJData {
    std::vector<Vertex> vertices;
    std::vector<Face>   faces;
    std::vector<UV>     UVs;
};

struct Material {
    std::string name;
    glm::vec3   ambient;
    glm::vec3   diffuse;
    glm::vec3   specular;
    float       shininess;
    float       transparency;
    std::string diffuseMapPath;
    int         illumination;
};

struct DDS_PIXELFORMAT {
    GLuint size;
    GLuint flags;
    GLuint fourCC;
    GLuint RGBBitCount;
    GLuint RBitMask;
    GLuint GBitMask;
    GLuint BBitMask;
    GLuint ABitMask;
};

struct DDS_HEADER {
    GLuint          size;
    GLuint          flags;
    GLuint          height;
    GLuint          width;
    GLuint          pitchOrLinearSize;
    GLuint          depth;
    GLuint          mipMapCount;
    GLuint          reserved1[11];
    DDS_PIXELFORMAT ddspf;
    GLuint          caps;
    GLuint          caps2;
    GLuint          caps3;
    GLuint          caps4;
    GLuint          reserved2;
};

OBJData                         loadFromOBJ(std::string& filename);
std::map<std::string, Material> loadMTLFile(std::string& filename);
std::vector<unsigned int>       flattenVertices(OBJData& meshData);
std::vector<UV>                 flattenUVs(OBJData& meshData);
GLuint                          loadDDSFile(const std::string& filename);

const GLuint DDS_MAGIC        = 0x20534444;  // "DDS " Header
const GLuint DDSD_MIPMAPCOUNT = 0x20000;
const GLuint DDPF_FOURCC      = 0x4;
const GLuint FOURCC_DXT1      = 0x31545844;  // "DXT1" in ASCII
const GLuint FOURCC_DXT3      = 0x33545844;  // "DXT3" in ASCII
const GLuint FOURCC_DXT5      = 0x35545844;  // "DXT5" in ASCII

#endif  // FILELOADER_H
