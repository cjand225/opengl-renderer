#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "ECS/Component.h"
#include "glm/glm.hpp"

class TransformComponent : public Component {
public:
    TransformComponent();
    ~TransformComponent();
};

#endif  // TRANSFORM_COMPONENT_H
