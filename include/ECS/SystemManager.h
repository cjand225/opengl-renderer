#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <memory>
#include <vector>

#include "ECS/System.h"

class SystemManager {
public:
    SystemManager();
    ~SystemManager();

    void registerSystem(System* system);
    void update();

private:
    std::vector<std::shared_ptr<System>> systems;
};

#endif  // SYSTEM_MANAGER_H
