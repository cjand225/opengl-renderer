#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unordered_map>

#include "Controls.h"
#include "FileLoader.h"
#include "Model.h"
#include "Shaders.h"

int main(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Failed to Initialize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL Renderer", NULL, NULL);

    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to Initialize GLEW" << std::endl;
        return -1;
    }
#endif

    // Print OpenGL version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version: " << version << std::endl;

#ifdef __APPLE__
    // Check for S3TC extension support on macOS
    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    if (!extensions || !strstr((const char*)extensions, "GL_EXT_texture_compression_s3tc")) {
        std::cerr << "Warning: S3TC texture compression not supported" << std::endl;
    }
#endif

    // Setup Event Polling
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Shader loading
    std::vector<std::pair<GLenum, std::string>> shaders = {
        {GL_VERTEX_SHADER, "./assets/shaders/vertex_shader.glsl"},
        {GL_FRAGMENT_SHADER, "./assets/shaders/fragment_shader.glsl"}};

    GLuint programID = LoadShaders(shaders);
    glUseProgram(programID);

    // Model Loading
    std::filesystem::path   modelPath   = "./assets/models/Goku/Goku.obj";
    std::string             mPath       = modelPath.string();
    OBJData                 meshData    = loadFromOBJ(mPath);
    std::vector<ModelGroup> modelGroups = Model::createFromOBJ(meshData);

    // Setup Control Variables
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    OrbitalCamera camera{
        .radius      = 50.0f,
        .theta       = 0.0f,
        .phi         = static_cast<float>(M_PI) / 4.0f,
        .target      = glm::vec3(0.0f, 20.0f, 0.0f),
        .minRadius   = 2.0f,
        .maxRadius   = 100.0f,
        .zoomSpeed   = 5.0f,
        .rotateSpeed = 1000.0f,
        .aspectRatio = float(width) / float(height)};

    // Setup callbacks
    glfwSetWindowUserPointer(window, &camera);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup MVP (Model View Project)
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model = glm::mat4(1.0f);

    calculateMatricies(window, camera, Projection, View, Model);
    glm::mat4 MVP = Projection * View * Model;

    // Get a handle for our "MVP" uniform
    GLuint MatrixID         = glGetUniformLocation(programID, "MVP");
    GLuint textureSamplerID = glGetUniformLocation(programID, "textureSampler");

    std::cout << "MVP uniform location: " << MatrixID << std::endl;
    std::cout << "Texture sampler uniform location: " << textureSamplerID << std::endl;

    // Setup before loop
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    for (const auto& group : meshData.materialGroups) {
        std::cout << "Material: " << group.first
                  << " Faces: " << group.second.size() << std::endl;
    }

    do {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poll and update the camera
        updateOrbitalCamera(window, camera);

        // Use the shader program
        glUseProgram(programID);

        // Recalculate Matrices
        calculateMatricies(window, camera, Projection, View, Model);
        MVP = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Render each material group
        for (const auto& group : modelGroups) {
            // Bind the texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, group.TextureID);
            glUniform1i(textureSamplerID, 0);

            // Bind VAO
            glBindVertexArray(group.VAO);

            // Draw the elements
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(group.indices.size()), GL_UNSIGNED_INT, 0);

            // Unbind VAO
            glBindVertexArray(0);
        }

        // Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup Model Groups
    for (const auto& group : modelGroups) {
        // Delete the VAO
        glDeleteVertexArrays(1, &group.VAO);

        if (group.VertexBuffer) {
            glDeleteBuffers(1, &group.VertexBuffer);
        }
        if (group.UVBuffer) {
            glDeleteBuffers(1, &group.UVBuffer);
        }
        if (group.ElementBuffer) {
            glDeleteBuffers(1, &group.ElementBuffer);
        }

        glDeleteTextures(1, &group.TextureID);
    }

    // Cleanup Programs
    glDeleteProgram(programID);

    // Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
