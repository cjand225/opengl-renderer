#include <cmath>

#include "Controls.h"

glm::vec3 sphericalToCartesian(float radius, float theta, float phi) {
    return glm::vec3(
        radius * sin(phi) * cos(theta),  // x
        radius * cos(phi),               // y
        radius * sin(phi) * sin(theta)   // z
    );
}

void updateOrbitalCamera(GLFWwindow* window, OrbitalCamera& camera) {
    static double lastTime = glfwGetTime();
    static double lastXpos = 0.0, lastYpos = 0.0;
    static bool   firstMouse = true;

    double currentTime = glfwGetTime();
    float  deltaTime   = float(currentTime - lastTime);
    lastTime           = currentTime;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse) {
            lastXpos   = xpos;
            lastYpos   = ypos;
            firstMouse = false;
        }

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float deltaTheta = camera.rotateSpeed * deltaTime * (float(lastXpos - xpos) / float(width));
        float deltaPhi   = camera.rotateSpeed * deltaTime * (float(lastYpos - ypos) / float(height));

        camera.theta += deltaTheta;
        camera.phi = glm::clamp(camera.phi + deltaPhi, 0.1f, static_cast<float>(M_PI - 0.1));

        lastXpos = xpos;
        lastYpos = ypos;
    } else {
        firstMouse = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Get the user pointer that was set
    OrbitalCamera* camera = static_cast<OrbitalCamera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->radius = glm::clamp(
            camera->radius - (float)yoffset * camera->zoomSpeed,
            camera->minRadius,
            camera->maxRadius);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Update viewport to new window size
    glViewport(0, 0, width, height);

    // Update the projection matrix in the camera
    OrbitalCamera* camera = static_cast<OrbitalCamera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Use the new aspect ratio
        camera->aspectRatio = float(width) / float(height);
    }
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

    projection = glm::perspective(glm::radians(45.0f), camera.aspectRatio, 0.1f, 100.0f);
    model      = glm::mat4(1.0f);
}
