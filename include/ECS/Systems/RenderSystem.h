#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "ECS/System.h"

class RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem();

    void initialize() override;
    void update() override;
    void cleanup() override;

    void setAspectRatio(float ratio);
    void updateViewport(int width, int height);

private:
    void render();
    void initializeShaders();
    void setupMVP();

    GLuint programID;
    GLuint matrixID;
    GLuint textureSamplerID;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 mvp;

    float aspectRatio;

    const std::vector<std::pair<GLenum, std::string>> shaderPaths = {
        {GL_VERTEX_SHADER, "./assets/shaders/vertex_shader.glsl"},
        {GL_FRAGMENT_SHADER, "./assets/shaders/fragment_shader.glsl"}};
};

#endif  // RENDER_SYSTEM_H
