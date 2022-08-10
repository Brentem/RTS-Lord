#pragma once

#include "raylib.h"

#include "Types.h"


class HudElement
{
    public:
        Vector2 position;
        Texture2D texture;
        
        HudElement(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY);
        void Draw();
};