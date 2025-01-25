#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "ECS/Component.h"
#include "Model.h"

class RenderComponent : public Component {
public:
    RenderComponent();
    ~RenderComponent();
};

#endif  // RENDER_COMPONENT_H
