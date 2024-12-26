#include <cmath>

#include "controls.h"

glm::vec3 sphericalToCartesian(float radius, float theta, float phi) {
    return glm::vec3(
        radius * sin(phi) * cos(theta),  // x
        radius * cos(phi),               // y
        radius * sin(phi) * sin(theta)   // z
    );
}

void updateOrbitalCamera(GLFWwindow* window, OrbitalCamera& camera) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float deltaTheta = camera.rotateSpeed * float(width / 2 - xpos);
    float deltaPhi   = camera.rotateSpeed * float(height / 2 - ypos);

    camera.theta += deltaTheta;
    camera.phi = glm::clamp(camera.phi + deltaPhi, 0.1f, static_cast<float>(M_PI - 0.1));

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.radius = glm::clamp(camera.radius - camera.zoomSpeed, camera.minRadius, camera.maxRadius);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.radius = glm::clamp(camera.radius + camera.zoomSpeed, camera.minRadius, camera.maxRadius);
    }

    glfwSetCursorPos(window, width / 2, height / 2);
}

glm::mat4 getOrbitalViewMatrix(const OrbitalCamera& camera) {
    glm::vec3 eye = sphericalToCartesian(camera.radius, camera.theta, camera.phi);
    return glm::lookAt(
        eye + camera.target,         // Camera position
        camera.target,               // Look at target
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );
}

void calculateMatricies(GLFWwindow* window, OrbitalCamera& camera, glm::mat4& projection, glm::mat4& view, glm::mat4& model) {
    updateOrbitalCamera(window, camera);

    glm::vec3 eye = sphericalToCartesian(camera.radius, camera.theta, camera.phi);
    view          = glm::lookAt(
        eye + camera.target,
        camera.target,
        glm::vec3(0.0f, 1.0f, 0.0f));

    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    model      = glm::mat4(1.0f);
}
