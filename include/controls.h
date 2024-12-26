#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct OrbitalCamera {
    float     radius = 50.0f;        // Distance from target
    float     theta  = 0.0f;         // Azimuthal angle (horizontal rotation)
    float     phi    = M_PI / 4.0f;  // Polar angle (vertical rotation)
    glm::vec3 target{0.0f};          // Look-at target (model center)
    float     minRadius   = 2.0f;    // Minimum zoom distance
    float     maxRadius   = 100.0f;  // Maximum zoom distance
    float     zoomSpeed   = 2.0f;    // Zoom sensitivity
    float     rotateSpeed = 0.005f;  // Rotation sensitivity
};

glm::mat4 getOrbitalViewMatrix(const OrbitalCamera& camera);
glm::vec3 sphericalToCartesian(float radius, float theta, float phi);
void      updateOrbitalCamera(GLFWwindow* window, OrbitalCamera& camera);
void      calculateMatricies(GLFWwindow* window, OrbitalCamera& camera, glm::mat4& projection, glm::mat4& view, glm::mat4& model);

#endif  // CONTROLS_H
