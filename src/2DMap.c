#include "../include/2DMap.h"

#include <stdlib.h>

MapInfo Map2D_Init(int mapWidth, int mapHeight)
{
	MapInfo mapInfo;

	mapInfo.mapWidth = mapWidth;
	mapInfo.mapHeight = mapHeight;
	mapInfo.position = (Vector2){((mapWidth/2) *-1), ((mapHeight/2) *-1)};

	return mapInfo;
}


Texture2D Map2DGetBackground(MapInfo info){

    // make a huge image for the map, a checkerboard with a red outline

	Image img = GenImageChecked(info.mapWidth, info.mapHeight, 32, 32, LIGHTGRAY, BEIGE);
	ImageDrawRectangleLines(&img, (Rectangle) { 0, 0, info.mapWidth, info.mapHeight }, 10, RED);
	ImageDrawCircle(&img, info.mapWidth/2, info.mapHeight/2, 50, GREEN);
	Texture2D background = LoadTextureFromImage(img);
	UnloadImage(img);

    return background;

}

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting)
{
	Boundaries boundaries = {0, 0, 0, 0};

	int verticalScrollSpace = info.mapHeight - setting.monitorHeight;
	boundaries.upperBoundary = info.position.y + (verticalScrollSpace/2);
	boundaries.lowerBoundary = info.position.y - (verticalScrollSpace/2);

	int horizontalScrollSpace = info.mapWidth - setting.monitorWidth;
	boundaries.leftBoundary = info.position.x + (horizontalScrollSpace/2);
	boundaries.rightBoundary = info.position.x - (horizontalScrollSpace/2);

	return boundaries;
}

void Map2D_HandleKeyboardInput(MapInfo* info)
{
	if(info == NULL)
	{
		return;
	}

	if (IsKeyDown(KEY_RIGHT)) info->position.x -= 5.0f;
    if (IsKeyDown(KEY_LEFT)) info->position.x += 5.0f;
    if (IsKeyDown(KEY_UP)) info->position.y += 5.0f;
    if (IsKeyDown(KEY_DOWN)) info->position.y -= 5.0f;
}

void Map2D_HandleMouseInput(MapInfo* info, MonitorSettings setting)
{
	if(info == NULL)
	{
		return;
	}
	
	int mouseX = GetMouseX();
	if (mouseX < 20) info->position.x += 2.0f;
    if (mouseX < 5) info->position.x += 4.0f;
    if (mouseX > setting.monitorWidth-20) info->position.x -= 2.0f;
    if (mouseX > setting.monitorWidth-5) info->position.x -= 4.0f;

	int mouseY = GetMouseY();
	if (mouseY < 20) info->position.y += 2.0f;
    if (mouseY < 5) info->position.y += 2.0f;
    if (mouseY > setting.monitorHeight-40) info->position.y -= 2.0f;
    if (mouseY > setting.monitorHeight-25) info->position.y -= 2.0f;
}

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries)
{
	if(info == NULL)
	{
		return;
	}

	if(info->position.y > boundaries.upperBoundary) info->position.y = boundaries.upperBoundary;
    if(info->position.y < boundaries.lowerBoundary) info->position.y = boundaries.lowerBoundary;
    if(info->position.x > boundaries.leftBoundary) info->position.x = boundaries.leftBoundary;
    if(info->position.x < boundaries.rightBoundary) info->position.x = boundaries.rightBoundary;
}
