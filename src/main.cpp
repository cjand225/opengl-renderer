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

#include "FileLoader.h"
#include "controls.h"
#include "shaders.h"

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

    // Material loading
    std::string                     materialPath   = "./assets/models/Goku/Goku.mtl";
    std::map<std::string, Material> modelMaterials = loadMTLFile(materialPath);

    // Model Loading
    std::filesystem::path     modelPath = "./assets/models/Goku/Goku.obj";
    std::string               mPath     = modelPath.string();
    OBJData                   meshData  = loadFromOBJ(mPath);
    std::vector<unsigned int> indices   = flattenVertices(meshData);
    std::vector<UV>           uvIndices = flattenUVs(meshData);

    // Texture Loading
    std::unordered_map<std::string, std::string> textures = {
        {"eyes.png", "./assets/models/Goku/results/eyes.DDS"},
        {"face.png", "./assets/models/Goku/results/face.DDS"},
        {"pants.png", "./assets/models/Goku/results/pants.DDS"},
        {"shirt.png", "./assets/models/Goku/results/shirt.DDS"}};

    std::unordered_map<std::string, GLuint> textureCache = {};
    for (const auto& pair : textures) {
        textureCache.insert(std::make_pair(pair.first, loadDDSFile(pair.second)));
    }

    // Setup VAO
    GLuint VertexBufferObject, ElementBufferObject, VertexArrayObject, UVBuffer;
    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);
    glGenBuffers(1, &UVBuffer);

    glBindVertexArray(VertexArrayObject);

    // Position Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), &meshData.vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvIndices.size() * sizeof(UV), &uvIndices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UV), (void*)0);
    glEnableVertexAttribArray(1);

    // Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);

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

    do {
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poll
        updateOrbitalCamera(window, camera);

        // Apply Shaders
        glUseProgram(programID);

        // Bind Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureCache["face.png"]);  // Try with face texture first
        glUniform1i(textureSamplerID, 0);

        // Recalculate Matricies

        calculateMatricies(window, camera, Projection, View, Model);

        MVP = Projection * View * Model;

        // Send Transformation to shader
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Draw our Model
        glBindVertexArray(VertexArrayObject);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup Buffers
    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);
    glDeleteBuffers(1, &UVBuffer);

    // Cleanup Programs
    glDeleteProgram(programID);

    // Cleanup Textures
    for (const auto& pair : textureCache) {
        glDeleteTextures(1, &pair.second);
    }

    // Cleanup VAO
    glDeleteVertexArrays(1, &VertexArrayObject);

    // Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
