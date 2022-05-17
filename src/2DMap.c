#include "../include/2DMap.h"

#include <stdlib.h>

MapInfo Map2D_Init(int rowCount, int columnCount, int cellSize)
{
	MapInfo mapInfo;

	mapInfo.rowCount = rowCount;
	mapInfo.columnCount = columnCount;
	mapInfo.cellSize = cellSize;
	mapInfo.mapWidth = columnCount * cellSize;
	mapInfo.mapHeight = rowCount * cellSize;
	mapInfo.position = (Vector2){((mapInfo.mapWidth /2) *-1), ((mapInfo.mapHeight/2) *-1)};

	return mapInfo;
}

Texture2D Map2DGetBackground(MapInfo info){

	// Texture2D spriteSheet = LoadTexture("../assets/spritesheet.png"); 
	Image spriteSheet = LoadImage("assets/spritesheet.png"); 

	Rectangle sprite1Rec = { 0.0f, 0.0f, 32.0f, 32.0f};
	Rectangle sprite2Rec = { 32.0f, 0.0f, 32.0f, 32.0f};
	Rectangle sprite3Rec = { 0.0f, 32.0f, 32.0f, 32.0f};
	Rectangle sprite4Rec = { 32.0f, 32.0f, 32.0f, 32.0f};

	Image imageBackground = GenImageColor(info.mapWidth, info.mapHeight, WHITE);
	// Random Grass
	for (int i = 0; i < info.rowCount; i++){
		for (int j = 0; j < info.columnCount; j++){
			int randomGrassTile = GetRandomValue(2, 4); 
			if(randomGrassTile == 2) ImageDraw(&imageBackground, spriteSheet, sprite2Rec,  (Rectangle) { j*32, i*32, 32, 32 }, WHITE);
			else if(randomGrassTile == 3) ImageDraw(&imageBackground, spriteSheet, sprite3Rec,  (Rectangle) { j*32, i*32, 32, 32 }, WHITE);
			else ImageDraw(&imageBackground, spriteSheet, sprite4Rec,  (Rectangle) { j*32, i*32, 32, 32 }, WHITE);
		}
	}

	// Random Dirt; 10 patches
	for (int i = 0; i < 10 ; i++){
		int posX = GetRandomValue(0, info.mapWidth);
		int posY = GetRandomValue(0, info.mapHeight);
		int width = GetRandomValue(6, 15);
		int height = GetRandomValue(6, 12);
			
		for (int y = 0; y < height ; y++){
			for (int x = 0; x < width ; x++){
				ImageDraw(&imageBackground, spriteSheet, sprite1Rec,  (Rectangle) {  posX + x*32, posY + y*32, 32, 32 }, WHITE);
			}
		}
	}

	ImageDrawRectangleLines(&imageBackground, (Rectangle) { 0, 0, info.mapWidth, info.mapHeight }, 4, RED);

	Texture2D background = LoadTextureFromImage(imageBackground);
	UnloadImage(imageBackground);

    return background;

}

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting, float zoomFactor)
{
	Boundaries boundaries = {0, 0, 0, 0};

	int verticalScrollSpace = info.mapHeight - (int)setting.monitorHeight/zoomFactor;
	boundaries.upperBoundary = info.position.y + (verticalScrollSpace/2);
	boundaries.lowerBoundary = info.position.y - (verticalScrollSpace/2);

	int horizontalScrollSpace = info.mapWidth - (int)setting.monitorWidth/zoomFactor;
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
	if (mouseX < 20) info->position.x += 4.0f;
    if (mouseX < 5) info->position.x += 12.0f;
    if (mouseX > setting.monitorWidth-20) info->position.x -= 4.0f;
    if (mouseX > setting.monitorWidth-5) info->position.x -= 12.0f;

	int mouseY = GetMouseY();
	if (mouseY < 20) info->position.y += 4.0f;
    if (mouseY < 5) info->position.y += 12.0f;
    if (mouseY > setting.monitorHeight-40) info->position.y -= 4.0f;
    if (mouseY > setting.monitorHeight-25) info->position.y -= 12.0f;
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
