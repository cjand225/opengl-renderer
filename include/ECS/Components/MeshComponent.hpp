#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include <glm/glm.hpp>
#include <vector>

#include "ECS/Component.h"

class MeshComponent : public Component {
public:
    MeshComponent() {}
    ~MeshComponent() {}

    std::vector<glm::vec3> vertices;

    std::vector<glm::vec3> normals;

    std::vector<glm::vec2> textureCoords;

    std::vector<unsigned int> indices;

    void addVertex(const glm::vec3& vertex) {
        vertices.push_back(vertex);
    }

    void addNormal(const glm::vec3& normal) {
        normals.push_back(normal);
    }

    void addTextureCoord(const glm::vec2& texCoord) {
        textureCoords.push_back(texCoord);
    }

    void addTriangle(unsigned int v1, unsigned int v2, unsigned int v3) {
        indices.push_back(v1);
        indices.push_back(v2);
        indices.push_back(v3);
    }

    void cleanup() {
        vertices.clear();
        normals.clear();
        textureCoords.clear();
        indices.clear();
    }
};

#endif  // MESH_COMPONENT_HPP
