#include "../include/UI.h"

UI UI_Init(int width, int height)
{
    UI ui;

    ui.texture = LoadTexture("assets/ui/UI_placeholder.png");

    ui.texture.height = height;
    ui.texture.width = width;

    return ui;
}