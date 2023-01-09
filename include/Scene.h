#pragma once

#include "../vendor/entt/entt.hpp"
#include <raylib.h>

class Scene
{
    public:
        Scene();
        ~Scene();
        entt::registry registry;
        int gold;

    private:
        // Functions underneath should not be in endproduct
        void CreatingWorkers();
        void CreatingBuildings();
        void CreatingResources();
};