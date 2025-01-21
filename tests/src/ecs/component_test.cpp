#include <gtest/gtest.h>

#include <memory>

#include "ECS/Component.h"
#include "ecs/mocks/mock_component.h"

// Component Tests
TEST(ComponentTest, ComponentLifecycle) {
    auto component = std::make_shared<MockComponent>();

    EXPECT_FALSE(component->initialized);
    EXPECT_FALSE(component->updated);
    EXPECT_FALSE(component->cleaned);

    component->initialize();
    EXPECT_TRUE(component->initialized);

    component->update();
    EXPECT_TRUE(component->updated);

    component->cleanup();
    EXPECT_TRUE(component->cleaned);
}
