#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <vector>

#include "FileLoader.h"

struct ModelGroup {
    GLuint                    VAO;
    GLuint                    VertexBuffer;
    GLuint                    UVBuffer;
    GLuint                    ElementBuffer;
    GLuint                    TextureID;
    std::string               materialName;
    std::vector<unsigned int> indices;
};

class Model {
public:
    Model(const OBJData& meshData);
    ~Model();
    static std::vector<ModelGroup> createFromOBJ(const OBJData& meshData);
    static GLuint                  createVAO(const std::vector<Vertex>&       vertices,
                                             const std::vector<UV>&           uvs,
                                             const std::vector<unsigned int>& indices);

    void draw(GLuint textureSamplerID) const;

private:
    std::vector<ModelGroup> modelGroups;

    void drawGroup(const ModelGroup& group, GLuint textureSamplerID) const;
};

#endif  // MODEL_H
