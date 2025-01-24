#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Controls.h"
#include "ECS/Systems/RenderSystem.h"
#include "Shaders.h"

RenderSystem::RenderSystem()
    : aspectRatio(4.0f / 3.0f) {}

RenderSystem::~RenderSystem() {
    cleanup();
}

void RenderSystem::initialize() {
    // Initialize OpenGL state
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    initializeShaders();
    setupMVP();
}

void RenderSystem::initializeShaders() {
    // Load and compile shaders
    programID = LoadShaders(shaderPaths);
    if (programID == 0) {
        std::cerr << "Failed to load shaders" << std::endl;
        return;
    }

    // Get uniform locations
    matrixID         = glGetUniformLocation(programID, "MVP");
    textureSamplerID = glGetUniformLocation(programID, "textureSampler");
}

void RenderSystem::setupMVP() {
    // Initialize matrices
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    view       = glm::lookAt(
              glm::vec3(0, 0, 50),  // Camera position
              glm::vec3(0, 0, 0),   // Look at origin
              glm::vec3(0, 1, 0)    // Head is up
          );
    model = glm::mat4(1.0f);  // Identity matrix

    // Calculate initial MVP
    mvp = projection * view * model;
}

void RenderSystem::setAspectRatio(float ratio) {
    aspectRatio = ratio;
    setupMVP();
}

void RenderSystem::updateViewport(int width, int height) {
    glViewport(0, 0, width, height);
    setAspectRatio(float(width) / float(height));
}

void RenderSystem::update() {
    render();
}

void RenderSystem::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader program
    glUseProgram(programID);

    // Update MVP matrix
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    // TODO: Implement ModelComponent
}

void RenderSystem::cleanup() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}
