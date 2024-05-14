#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

glm::vec2 getMousePosition(GLFWwindow* window);

void calculatePosition(GLFWwindow* window, glm::vec3& position, const glm::vec3& direction, const glm::vec3& right, float& deltaTime, float& speed);

#endif // CONTROLS_H 