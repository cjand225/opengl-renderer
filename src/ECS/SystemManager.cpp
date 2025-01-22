#include "ECS/SystemManager.h"

SystemManager::SystemManager() {
    systems.clear();
    systems.reserve(10);
}

SystemManager::~SystemManager() {
    for (auto& system : systems) {
        system->cleanup();
    }
    systems.clear();
}

void SystemManager::registerSystem(System* system) {
    systems.push_back(std::shared_ptr<System>(system));
}

void SystemManager::update() {
    for (auto& system : systems) {
        system->update();
    }
}
