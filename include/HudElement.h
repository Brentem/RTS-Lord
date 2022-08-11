#pragma once

extern "C"
{
    #include "raylib.h"
}

class HudElement
{
    protected:
        Texture2D texture;

    public:
        Vector2 position;
        
        HudElement(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY);
        virtual void Draw();
};