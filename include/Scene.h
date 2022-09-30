#pragma once

#include "../vendor/entt/entt.hpp"
#include <raylib.h>

class Scene
{
    public:
        Scene(Texture2D characterTexture);
        ~Scene();
        entt::registry registry;

    private:
        // Functions underneath should not be in endproduct
        void CreatingWorkers(Texture2D characterTexture);
        void CreatingBuildings();
        void CreatingResources();
};