#include <gtest/gtest.h>

#include <memory>

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ecs/mocks/mock_component.h"

// Template specializations for MockComponent
template <>
void Entity::addComponent<MockComponent>(std::shared_ptr<MockComponent> component) {
    components[std::type_index(typeid(MockComponent))] = component;
}

template <>
std::shared_ptr<MockComponent> Entity::getComponent<MockComponent>() {
    auto it = components.find(std::type_index(typeid(MockComponent)));
    if (it != components.end()) {
        return std::static_pointer_cast<MockComponent>(it->second);
    }
    return nullptr;
}

template <>
void Entity::removeComponent<MockComponent>() {
    components.erase(std::type_index(typeid(MockComponent)));
}

// Entity Tests
class EntityTest : public ::testing::Test {
protected:
    std::unique_ptr<Entity> entity;

    void SetUp() override {
        entity = std::make_unique<Entity>("TestEntity", 1);
    }
};

TEST_F(EntityTest, EntityConstruction) {
    EXPECT_EQ(entity->getName(), "TestEntity");
    EXPECT_EQ(entity->getID(), 1);
}

TEST_F(EntityTest, ComponentManagement) {
    auto component = std::make_shared<MockComponent>();

    // Test adding component
    entity->addComponent<MockComponent>(component);
    auto retrieved = entity->getComponent<MockComponent>();
    EXPECT_EQ(retrieved, component);

    // Test removing component
    entity->removeComponent<MockComponent>();
    retrieved = entity->getComponent<MockComponent>();
    EXPECT_EQ(retrieved, nullptr);
}

TEST_F(EntityTest, ComponentDestruction) {
    auto component = std::make_shared<MockComponent>();
    entity->addComponent<MockComponent>(component);

    // Test component is properly destroyed when entity is cleaned up
    entity->cleanup();
    EXPECT_EQ(entity->getComponent<MockComponent>(), nullptr);
}

TEST_F(EntityTest, MultipleComponents) {
    auto component1 = std::make_shared<MockComponent>();
    auto component2 = std::make_shared<MockComponent>();

    // Add first component
    entity->addComponent<MockComponent>(component1);
    auto found1 = entity->getComponent<MockComponent>();
    EXPECT_EQ(found1, component1);

    // Replace with second component
    entity->addComponent<MockComponent>(component2);
    auto found2 = entity->getComponent<MockComponent>();
    EXPECT_EQ(found2, component2);
    EXPECT_NE(found2, component1);  // Ensure it's not the first component

    // Remove component and verify it's gone
    entity->removeComponent<MockComponent>();
    auto found3 = entity->getComponent<MockComponent>();
    EXPECT_EQ(found3, nullptr);
}
