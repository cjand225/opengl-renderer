#include <gtest/gtest.h>

#include <memory>

#include "ECS/Component.h"
#include "ecs/mocks/mock_component.h"

// Component Tests
TEST(ComponentTest, ComponentLifecycle) {
    MockComponent* rawPtr = new MockComponent();
    {
        auto component = std::shared_ptr<MockComponent>(rawPtr);
        EXPECT_FALSE(rawPtr->destroyed);
    }
    EXPECT_TRUE(rawPtr->destroyed);
}

TEST(ComponentTest, ComponentCreation) {
    auto component = std::make_shared<MockComponent>();
    EXPECT_NE(component, nullptr);
}
