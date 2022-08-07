#include "../include/UI.h"

UI UI_Init(int width, int height)
{
    UI ui;

    ui.texture = LoadTexture("assets/ui/UI_placeholder.png");

    ui.texture.height = height;
    ui.texture.width = width;

    return ui;
}

void UI_Draw(UI ui, Camera2D camera)
{
    float posX = (camera.offset.x / camera.zoom) * -1;
    float posY = UI_POSY_MARGIN - (camera.offset.y / camera.zoom);

    DrawTexture(ui.texture, posX, posY, WHITE);
}