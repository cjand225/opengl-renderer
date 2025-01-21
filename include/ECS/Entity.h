#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "Component.h"

class Entity {
public:
    Entity(const std::string& name, unsigned int id);
    ~Entity();

    template <typename T>
    void addComponent(std::shared_ptr<T> component);

    template <typename T>
    void removeComponent();

    template <typename T>
    std::shared_ptr<T> getComponent();

    const std::string& getName() const {
        return name;
    }

    unsigned int getID() const {
        return id;
    }

    void cleanup();

private:
    std::string                                                     name;
    unsigned int                                                    id;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

#endif  // ENTITY_H
