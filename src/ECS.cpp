#include "../include/ECS.h"

#include <vector>

std::vector<int> entities = {0, 1, 2, 3, 4};
std::vector<Vector2> positions = {{10, 10}, {50, 50}, {100, 100}, {150, 150}, {1000, 1000}};

static Rectangle frameRec = { 0.0f, 0.0f, 32.0f, 32.0f };

void DrawEntities(Texture2D characterTexture, MapInfo info)
{
    for(unsigned int i = 0; i < entities.size(); i++)
    {
        Vector2 newPosition = positions[entities[i]];
        newPosition.x += info.position.x;
        newPosition.y += info.position.y;

        DrawTextureRec(characterTexture, frameRec, newPosition, WHITE);
    }
}