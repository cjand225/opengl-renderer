#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::vec2 getMousePosition(GLFWwindow* window);

glm::vec3 calculateDirection(const float& verticleAngle, const float& horizontalAngle);
void calculatePosition(GLFWwindow* window, glm::vec3& position, const glm::vec3& direction, const glm::vec3& right, const float& deltaTime, const float& speed);

#endif // CONTROLS_H 