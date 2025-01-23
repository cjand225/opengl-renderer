#include <gtest/gtest.h>

#include <glm/gtc/constants.hpp>
#include <memory>

#include "ECS/Components/TransformComponent.hpp"

class TransformComponentTest : public ::testing::Test {
protected:
    std::shared_ptr<TransformComponent> transform;

    void SetUp() override {
        transform = std::make_shared<TransformComponent>(
            glm::vec3(0.0f),  // position
            glm::vec3(1.0f),  // scale
            glm::vec3(0.0f)   // rotation
        );
    }
};

TEST_F(TransformComponentTest, Construction) {
    EXPECT_EQ(transform->position, glm::vec3(0.0f));
    EXPECT_EQ(transform->scale, glm::vec3(1.0f));
    EXPECT_EQ(transform->rotation, glm::vec3(0.0f));
}

TEST_F(TransformComponentTest, TransformMatrix) {
    // Test identity transform
    glm::mat4 identity = transform->getTransformMatrix();
    EXPECT_EQ(identity, glm::mat4(1.0f));

    // Test translation
    transform->position  = glm::vec3(1.0f, 2.0f, 3.0f);
    glm::mat4 translated = transform->getTransformMatrix();
    glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformedPoint = translated * point;
    EXPECT_EQ(transformedPoint, glm::vec4(1.0f, 2.0f, 3.0f, 1.0f));

    // Test scaling
    transform->position = glm::vec3(0.0f);
    transform->scale    = glm::vec3(2.0f);
    glm::mat4 scaled    = transform->getTransformMatrix();
    point               = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    transformedPoint    = scaled * point;
    EXPECT_EQ(transformedPoint, glm::vec4(2.0f, 2.0f, 2.0f, 1.0f));

    // Test rotation (90 degrees around Z-axis)
    transform->scale    = glm::vec3(1.0f);
    transform->rotation = glm::vec3(0.0f, 0.0f, glm::half_pi<float>());
    glm::mat4 rotated   = transform->getTransformMatrix();
    point               = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    transformedPoint    = rotated * point;
    // After 90-degree rotation around Z, (1,0,0) should become approximately (0,1,0)
    EXPECT_NEAR(transformedPoint.x, 0.0f, 0.0001f);
    EXPECT_NEAR(transformedPoint.y, 1.0f, 0.0001f);
    EXPECT_NEAR(transformedPoint.z, 0.0f, 0.0001f);
}

TEST_F(TransformComponentTest, CombinedTransformations) {
    // Test combination of translation, rotation, and scale
    transform->position = glm::vec3(1.0f, 0.0f, 0.0f);
    transform->scale    = glm::vec3(2.0f);
    transform->rotation = glm::vec3(0.0f, 0.0f, glm::half_pi<float>());

    glm::mat4 combined = transform->getTransformMatrix();
    glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformedPoint = combined * point;

    // Point should be scaled by 2, rotated 90 degrees, then translated by (1,0,0)
    EXPECT_NEAR(transformedPoint.x, 1.0f, 0.0001f);
    EXPECT_NEAR(transformedPoint.y, 2.0f, 0.0001f);
    EXPECT_NEAR(transformedPoint.z, 0.0f, 0.0001f);
}
