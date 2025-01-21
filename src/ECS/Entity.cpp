#include "ECS/Entity.h"

Entity::Entity(const std::string& name, unsigned int id)
    : name(name), id(id) {
}

Entity::~Entity() {
    cleanup();
}

void Entity::cleanup() {
    for (auto& [type, component] : components) {
        component->cleanup();
    }
    components.clear();
}

template <typename T>
void Entity::addComponent(std::shared_ptr<T> component) {
    components[std::type_index(typeid(T))] = component;
}

template <typename T>
void Entity::removeComponent() {
    components.erase(std::type_index(typeid(T)));
}

template <typename T>
std::shared_ptr<T> Entity::getComponent() {
    auto it = components.find(std::type_index(typeid(T)));
    if (it != components.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }
    return nullptr;
}
