#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::vec2 getMousePosition(GLFWwindow* window);

float     calculateAngle(GLFWwindow* window, double& position);
glm::vec3 calculateDirection(const float& verticleAngle, const float& horizontalAngle);
glm::vec3 calculateRightHand(const float& horizontalAngle);
void      calculatePosition(GLFWwindow* window, glm::vec4& position, const glm::vec4& direction, const glm::vec4& right, const float& deltaTime, const float& speed);

void calculateMatricies(GLFWwindow* window, glm::mat4& projection, glm::mat4& view, glm::mat4& model, float& horizontalAngle, float& verticalAngle, const float& mouseSpeed, const float& initialFieldOfView, glm::vec3& position, const float& speed);

#endif  // CONTROLS_H
