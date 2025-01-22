#ifndef MOCK_COMPONENT_H
#define MOCK_COMPONENT_H

#include "ECS/Component.h"

class MockComponent : public Component {
public:
    // We'll keep track of when the component is destroyed
    bool destroyed = false;

    ~MockComponent() override {
        destroyed = true;
    }
};

#endif  // MOCK_COMPONENT_H
