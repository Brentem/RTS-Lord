#include "../include/HudElement.h"

HudElement::HudElement(Texture2D* texture, Camera2D cam, int width, int height, int marginX, int marginY)
{
    this->texture = texture;
    this->width = width;
    this->height = height;

    position.x = marginX - (cam.offset.x / cam.zoom);
    position.y = marginY - (cam.offset.y / cam.zoom);
}

void HudElement::Draw()
{
    texture->width = width;
    texture->height = height;

    DrawTexture(*texture, position.x, position.y, WHITE);
}