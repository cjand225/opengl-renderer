#ifndef MOCK_SYSTEM_H
#define MOCK_SYSTEM_H

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

#endif  // MOCK_SYSTEM_H
