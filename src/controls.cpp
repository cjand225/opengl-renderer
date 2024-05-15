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

float calculateAngle(GLFWwindow* window, double& position)
{
    return 0.0f;
}

glm::vec3 calculateRightHand(const float& horizontalAngle)
{
    return glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );
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
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ){
		position -= right * deltaTime * speed;
	}
}

void calculateMatricies(GLFWwindow* window, glm::mat4& projection, glm::mat4& view, glm::mat4& model, float& horizontalAngle, float& verticalAngle, const float& mouseSpeed, const float& initialFieldOfView, glm::vec3& position, const float& speed)
{
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get Mouse Position
    glm::vec2 mousePosition = getMousePosition(window);
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Calculate Angles
    horizontalAngle += mouseSpeed * float(1024/2 - mousePosition.x);
    verticalAngle += mouseSpeed * float(768/2 - mousePosition.y);

    // Calculate Vectors
    glm::vec3 direction = calculateDirection(verticalAngle, horizontalAngle);
    glm::vec3 right = calculateRightHand(horizontalAngle);
    glm::vec3 up = glm::cross(right, direction);
    calculatePosition(window, position, direction, right, deltaTime, speed);

    float fieldOfView = initialFieldOfView;

    projection = glm::perspective(glm::radians(fieldOfView), 4.0f / 3.0f, 0.1f, 100.0f);

    view = glm::lookAt(
        position,
        position+direction,
        up
    );

}
