#pragma once

extern "C"
{
    #include "raylib.h"
}

#include <string>

class HudElement
{
    protected:
        std::string textureName;
        int width;
        int height;

    public:
        Vector2 position;
        
        HudElement(std::string textureName, Camera2D cam, int width, int height, int marginX, int marginY);
        virtual ~HudElement() = default;
        virtual void Draw();
};