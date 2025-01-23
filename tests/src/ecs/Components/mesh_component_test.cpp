#include <gtest/gtest.h>

#include <memory>

#include "ECS/Components/MeshComponent.hpp"

class MeshComponentTest : public ::testing::Test {
protected:
    std::shared_ptr<MeshComponent> mesh;

    void SetUp() override {
        mesh = std::make_shared<MeshComponent>();
    }
};

TEST_F(MeshComponentTest, Construction) {
    EXPECT_TRUE(mesh->vertices.empty());
    EXPECT_TRUE(mesh->normals.empty());
    EXPECT_TRUE(mesh->textureCoords.empty());
    EXPECT_TRUE(mesh->indices.empty());
}

TEST_F(MeshComponentTest, AddVertex) {
    glm::vec3 vertex(1.0f, 2.0f, 3.0f);
    mesh->addVertex(vertex);

    EXPECT_EQ(mesh->vertices.size(), 1);
    EXPECT_EQ(mesh->vertices[0], vertex);
}

TEST_F(MeshComponentTest, AddNormal) {
    glm::vec3 normal(0.0f, 1.0f, 0.0f);
    mesh->addNormal(normal);

    EXPECT_EQ(mesh->normals.size(), 1);
    EXPECT_EQ(mesh->normals[0], normal);
}

TEST_F(MeshComponentTest, AddTextureCoord) {
    glm::vec2 texCoord(0.5f, 0.5f);
    mesh->addTextureCoord(texCoord);

    EXPECT_EQ(mesh->textureCoords.size(), 1);
    EXPECT_EQ(mesh->textureCoords[0], texCoord);
}

TEST_F(MeshComponentTest, AddTriangle) {
    mesh->addTriangle(0, 1, 2);

    EXPECT_EQ(mesh->indices.size(), 3);
    EXPECT_EQ(mesh->indices[0], 0u);
    EXPECT_EQ(mesh->indices[1], 1u);
    EXPECT_EQ(mesh->indices[2], 2u);
}

TEST_F(MeshComponentTest, ComplexMesh) {
    // Add multiple vertices
    mesh->addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
    mesh->addVertex(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->addVertex(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->addVertex(glm::vec3(1.0f, 1.0f, 0.0f));

    // Add corresponding normals
    mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));

    // Add texture coordinates
    mesh->addTextureCoord(glm::vec2(0.0f, 0.0f));
    mesh->addTextureCoord(glm::vec2(1.0f, 0.0f));
    mesh->addTextureCoord(glm::vec2(0.0f, 1.0f));
    mesh->addTextureCoord(glm::vec2(1.0f, 1.0f));

    // Add two triangles to form a quad
    mesh->addTriangle(0, 1, 2);
    mesh->addTriangle(1, 3, 2);

    EXPECT_EQ(mesh->vertices.size(), 4);
    EXPECT_EQ(mesh->normals.size(), 4);
    EXPECT_EQ(mesh->textureCoords.size(), 4);
    EXPECT_EQ(mesh->indices.size(), 6);
}

TEST_F(MeshComponentTest, Cleanup) {
    // Add some data
    mesh->addVertex(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->addNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->addTextureCoord(glm::vec2(0.5f, 0.5f));
    mesh->addTriangle(0, 1, 2);

    // Verify data was added
    EXPECT_FALSE(mesh->vertices.empty());
    EXPECT_FALSE(mesh->normals.empty());
    EXPECT_FALSE(mesh->textureCoords.empty());
    EXPECT_FALSE(mesh->indices.empty());

    // Clean up
    mesh->cleanup();

    // Verify all data was cleared
    EXPECT_TRUE(mesh->vertices.empty());
    EXPECT_TRUE(mesh->normals.empty());
    EXPECT_TRUE(mesh->textureCoords.empty());
    EXPECT_TRUE(mesh->indices.empty());
}
