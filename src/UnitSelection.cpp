#include "../include/UnitSelection.h"

#include "../include/Types.h"

UnitSelection::UnitSelection(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY)
: HudElement(texture, cam, width, height, marginX, marginY)
{
    selectedUnits = 0;
}

void UnitSelection::Draw()
{
    Vector2 defaultPosition = position;

    texture.width = width;
    texture.height = height;

    for(int i = 0; i < selectedUnits; i++)
    {
        if(i == MAX_UNITS_SELECTED/2)
        {
            defaultPosition.y += 32;
            defaultPosition.x = position.x;
        }

        DrawTexture(texture, defaultPosition.x, defaultPosition.y, WHITE);
        defaultPosition.x += 32;
    }
}