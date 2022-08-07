#include "../include/UI.h"

UI::UI(Texture2D texture, float widthFactor, float heightFactor)
{
    Texture = texture;
    WidthFactor = widthFactor;
    HeightFactor = heightFactor;
}

void UI::Update(Camera2D camera)
{
    int screenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();
    float zoomPercentage = (camera.zoom - 1.0);
    Texture.width = screenWidth * zoomPercentage * WidthFactor;
    Texture.height = ScreenHeight * zoomPercentage * HeightFactor;
    //Texture.height = UI_LENGTH;
}

void UI::Draw(Camera2D camera)
{
    float posX = (camera.offset.x / camera.zoom) * -1;
    float posY = UI_POSY_MARGIN - (camera.offset.y / camera.zoom);

    DrawTexture(Texture, posX, posY, WHITE);
}