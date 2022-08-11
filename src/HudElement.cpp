#include "../include/HudElement.h"

HudElement::HudElement(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY)
{
    this->texture = texture;
    this->texture.width = width;
    this->texture.height = height;

    position.x = marginX - (cam.offset.x / cam.zoom);
    position.y = marginY - (cam.offset.y / cam.zoom);
}

void HudElement::Draw()
{
    DrawTexture(texture, position.x, position.y, WHITE);
}