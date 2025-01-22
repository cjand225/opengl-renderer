#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "ECS/System.h"

class RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem();

    void initialize() override;
    void update() override;
    void cleanup() override;

private:
    void render();
};
#endif  // RENDER_SYSTEM_H
