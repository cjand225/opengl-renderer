#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "ECS/Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class TransformComponent : public Component {
public:
    TransformComponent(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation)
        : position(pos),
          scale(scale),
          rotation(rotation) {}

    ~TransformComponent() {}

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    glm::mat4 getTransformMatrix() const {
        glm::mat4 transform = glm::mat4(1.0f);
        transform           = glm::translate(transform, position);
        transform           = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        transform           = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        transform           = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        transform           = glm::scale(transform, scale);
        return transform;
    }
};

#endif  // TRANSFORM_COMPONENT_HPP
