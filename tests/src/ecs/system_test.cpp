#include <gtest/gtest.h>

#include <memory>

#include "ECS/System.h"

class MockSystem : public System {
public:
    bool initialized = false;
    bool updated     = false;
    bool cleaned     = false;

    void initialize() override {
        initialized = true;
    }
    void update() override {
        updated = true;
    }
    void cleanup() override {
        cleaned = true;
    }
};

// System Tests
TEST(SystemTest, SystemLifecycle) {
    auto system = std::make_unique<MockSystem>();

    EXPECT_FALSE(system->initialized);
    EXPECT_FALSE(system->updated);
    EXPECT_FALSE(system->cleaned);

    system->initialize();
    EXPECT_TRUE(system->initialized);

    system->update();
    EXPECT_TRUE(system->updated);

    system->cleanup();
    EXPECT_TRUE(system->cleaned);
}
