#include "../include/UnitSelection.h"

UnitSelection::UnitSelection(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY)
: HudElement(texture, cam, width, height, marginX, marginY)
{}

void UnitSelection::Draw()
{
    Vector2 defaultPosition = position;

    texture.width = width;
    texture.height = height;

    int amount = 30;

    for(int i = 0; i < amount; i++)
    {
        if(i == amount/2)
        {
            defaultPosition.y += 32;
            defaultPosition.x = position.x;
        }

        DrawTexture(texture, defaultPosition.x, defaultPosition.y, WHITE);
        defaultPosition.x += 32;
    }
}