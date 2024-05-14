#include "controls.h"

glm::mat4 getProjectionMatrix()
{
    return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
}

glm::mat4 getViewMatrix()
{
    // Camera
    return glm::lookAt(
        glm::vec3(40, 40, 10),  // Camera Location in World Space
        glm::vec3(-40, 0, 0),   // Look at Origin, 40 units to right 
        glm::vec3(0, 1, 0)      // Head is up
    );
}