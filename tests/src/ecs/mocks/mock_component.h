#ifndef MOCK_COMPONENT_H
#define MOCK_COMPONENT_H

#include "ECS/Component.h"

class MockComponent : public Component {
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

#endif  // MOCK_COMPONENT_H
