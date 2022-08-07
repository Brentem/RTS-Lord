#pragma once

#include "Types.h"

#define UI_POSY_MARGIN -20.0f

struct UI
{
    Texture2D Texture;
    float WidthFactor;
    float HeightFactor;

    UI(Texture2D texture, float widthFactor, float heightFactor);
    void Update(Camera2D camera);
    void Draw(Camera2D camera);
};