#pragma once

#include <entt/entt.hpp>
#include <raylib.h>

class Scene
{
    public:
        Scene(Texture2D characterTexture);
        ~Scene();
        entt::registry registry;
};