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

glm::vec2 getMousePosition(GLFWwindow* window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

glm::vec3 calculateDirection(const float& verticleAngle, const float& horizontalAngle)
{
    return glm::vec3(
        cos(verticleAngle) * sin(horizontalAngle),
        sin(verticleAngle),
        cos(verticleAngle) * cos(horizontalAngle)
    );
}

void calculatePosition(GLFWwindow* window, glm::vec3& position, const glm::vec3& direction, const glm::vec3& right, const float& deltaTime, const float& speed)
{
    // Move Forward
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
}

