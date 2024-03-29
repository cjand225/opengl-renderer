
#include "OBJLoader.h"
#include "shaders.h"

#include <filesystem>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv) {
 
    if(!glfwInit())
    {
        std::cerr << "Failed to Initialize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL Renderer", NULL, NULL);

    if(!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to Initialize GLEW" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    std::vector<std::pair<GLenum, std::string>> shaders = {
        {GL_VERTEX_SHADER, "../../assets/shaders/vertex_shader.glsl"},
        {GL_FRAGMENT_SHADER, "../../assets/shaders/fragment_shader.glsl"}
    };

    GLuint programID = LoadShaders(shaders);

    // Model Loading
    std::filesystem::path modelPath = "../../assets/models/Goku/Goku.obj";
    OBJData meshData = loadFromOBJ(modelPath.string());
    std::vector<unsigned int> indices = flattenVertices(meshData);

    // Setup VAO
    GLuint VertexBufferObject, ElementBufferObject, VertexArrayObject;
    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), &meshData.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);


    // Setup GLM

    // Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera
    glm::mat4 View = glm::lookAt(
        glm::vec3(40, 40, 10), // Camera Location
        glm::vec3(-40, 0, 0), // Look at Origin
        glm::vec3(0, 1, 0) // Head is up
    );

    glm ::mat4 Model = glm::mat4(1.0f);
    glm::mat4 MVP = Projection * View * Model;

    // Setup before loop
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    do
    {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply Shaders
        glUseProgram(programID);

        // Send Transformation to shader
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        //Draw something
        glBindVertexArray(VertexArrayObject);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    return 0;
}
