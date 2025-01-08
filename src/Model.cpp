#include "Model.h"

std::vector<ModelGroup> Model::createFromOBJ(const OBJData& meshData) {
    std::vector<ModelGroup> modelGroups;

    for (const auto& group : meshData.materialGroups) {
        const std::string&       materialName = group.first;
        const std::vector<Face>& faces        = group.second;

        std::vector<Vertex>       groupVertices;
        std::vector<UV>           groupUVs;
        std::vector<unsigned int> groupIndices;

        flattenGroupData(faces, meshData.vertices, meshData.UVs,
                         groupVertices, groupUVs, groupIndices);

        GLuint textureID  = 0;
        auto   materialIt = meshData.materials.find(materialName);
        if (materialIt != meshData.materials.end() && materialIt->second.texture) {
            textureID = materialIt->second.texture;
        }

        GLuint     VAO = createVAO(groupVertices, groupUVs, groupIndices);
        ModelGroup modelGroup{VAO, 0, 0, 0, textureID, materialName, groupIndices};
        modelGroups.push_back(modelGroup);
    }

    return modelGroups;
}

GLuint Model::createVAO(const std::vector<Vertex>& vertices, const std::vector<UV>& uvs, const std::vector<unsigned int>& indices) {
    GLuint VAO, VBO, UVBuffer, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &UVBuffer);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // UV Buffer
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(UV), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UV), (void*)0);
    glEnableVertexAttribArray(1);

    // Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return VAO;
}
