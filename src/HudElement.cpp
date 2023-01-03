#include "../include/HudElement.h"

#include "../include/AssetManager.h"

using namespace std;

HudElement::HudElement(string textureName, Camera2D cam, int width, int height, int marginX, int marginY)
{
    this->textureName = textureName;
    this->width = width;
    this->height = height;

    position.x = marginX - (cam.offset.x / cam.zoom);
    position.y = marginY - (cam.offset.y / cam.zoom);
}

void HudElement::Draw()
{
    AssetManager* assetManager = AssetManager::GetInstance();

    if(assetManager)
    {
        Texture texture = assetManager->GetTextureUI(textureName);
        texture.width = width;
        texture.height = height;

        DrawTexture(texture, position.x, position.y, WHITE);
    }
}