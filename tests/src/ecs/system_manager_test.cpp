#include <gtest/gtest.h>

#include "ECS/SystemManager.h"
#include "ecs/mocks/mock_system.h"

class SystemManagerTest : public ::testing::Test {
protected:
    std::unique_ptr<SystemManager> systemManager;

    void SetUp() override {
        systemManager = std::make_unique<SystemManager>();
    }
};

TEST_F(SystemManagerTest, SystemRegistration) {
    auto* mockSystem = new MockSystem();
    systemManager->registerSystem(mockSystem);

    // Update should propagate to registered system
    systemManager->update();
    EXPECT_TRUE(mockSystem->updated);
}

TEST_F(SystemManagerTest, SystemCleanup) {
    auto* mockSystem1 = new MockSystem();
    auto* mockSystem2 = new MockSystem();

    systemManager->registerSystem(mockSystem1);
    systemManager->registerSystem(mockSystem2);

    // Destructor should call cleanup on all systems
    systemManager.reset();

    EXPECT_TRUE(mockSystem1->cleaned);
    EXPECT_TRUE(mockSystem2->cleaned);
}

TEST_F(SystemManagerTest, MultipleSystemUpdates) {
    auto* mockSystem1 = new MockSystem();
    auto* mockSystem2 = new MockSystem();

    systemManager->registerSystem(mockSystem1);
    systemManager->registerSystem(mockSystem2);

    systemManager->update();

    EXPECT_TRUE(mockSystem1->updated);
    EXPECT_TRUE(mockSystem2->updated);
}
