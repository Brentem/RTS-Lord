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
	mapInfo.position = (Vector2){(((float)mapInfo.mapWidth /2) *-1), (((float)mapInfo.mapHeight/2) *-1)};

	return mapInfo;
}

Texture2D Map2DGetBackground(MapInfo mapInfo){

	// Texture2D spriteSheet = LoadTexture("../assets/spritesheet.png"); 
	Image spriteSheet = LoadImage("assets/spritesheet.png"); 

	Rectangle sprite1Rec = { 0.0f, 0.0f, 32.0f, 32.0f};
	Rectangle sprite2Rec = { 32.0f, 0.0f, 32.0f, 32.0f};
	Rectangle sprite3Rec = { 0.0f, 32.0f, 32.0f, 32.0f};
	Rectangle sprite4Rec = { 32.0f, 32.0f, 32.0f, 32.0f};

	Image imageBackground = GenImageColor(mapInfo.mapWidth, mapInfo.mapHeight, WHITE);
	// Random Grass
	for (int i = 0; i < mapInfo.rowCount; i++){
		for (int j = 0; j < mapInfo.columnCount; j++){
			int randomGrassTile = GetRandomValue(2, 4); 
			if(randomGrassTile == 2) ImageDraw(&imageBackground, spriteSheet, sprite2Rec,  (Rectangle) { (float)j*32, (float)i*32, 32.0f, 32.0f }, WHITE);
			else if(randomGrassTile == 3) ImageDraw(&imageBackground, spriteSheet, sprite3Rec,  (Rectangle) { (float)j*32, (float)i*32, 32.0f, 32.0f }, WHITE);
			else ImageDraw(&imageBackground, spriteSheet, sprite4Rec,  (Rectangle) { (float)j*32, (float)i*32, 32.0f, 32.0f }, WHITE);
		}
	}

	// Random Dirt; 10 patches
	for (int i = 0; i < 10 ; i++){
		int posX = GetRandomValue(0, mapInfo.mapWidth);
		int posY = GetRandomValue(0, mapInfo.mapHeight);
		int width = GetRandomValue(6, 15);
		int height = GetRandomValue(6, 12);
			
		for (int y = 0; y < height ; y++){
			for (int x = 0; x < width ; x++){
				ImageDraw(&imageBackground, spriteSheet, sprite1Rec,  (Rectangle) { (float)(posX + x*32), (float)(posY + y*32), 32.0f, 32.0f }, WHITE);
			}
		}
	}

	ImageDrawRectangleLines(&imageBackground, (Rectangle) { 0, 0, (float)mapInfo.mapWidth, (float)mapInfo.mapHeight }, 4, RED);

	Texture2D background = LoadTextureFromImage(imageBackground);
	UnloadImage(imageBackground);

    return background;

}

Boundaries Map2D_GetBoundaries(MapInfo mapInfo, MonitorSettings monitorSettings, float zoomFactor)
{
	Boundaries boundaries = {0, 0, 0, 0};

	int verticalScrollSpace = mapInfo.mapHeight - (int)monitorSettings.monitorHeight/zoomFactor;
	boundaries.upperBoundary = mapInfo.position.y + (verticalScrollSpace/2);
	boundaries.lowerBoundary = mapInfo.position.y - (verticalScrollSpace/2);

	int horizontalScrollSpace = mapInfo.mapWidth - (int)monitorSettings.monitorWidth/zoomFactor;
	boundaries.leftBoundary = mapInfo.position.x + (horizontalScrollSpace/2);
	boundaries.rightBoundary = mapInfo.position.x - (horizontalScrollSpace/2);
	return boundaries;
}

static void SetOffset(MapInfo* mapInfo){
	if(mapInfo == NULL)
	{
		return;
	}

	mapInfo->offSet.x = mapInfo->mapWidth/2 + mapInfo->position.x;
	mapInfo->offSet.y = mapInfo->mapHeight/2 + mapInfo->position.y;
}

void Map2D_HandleKeyboardInput(MapInfo* mapInfo)
{
	if(mapInfo == NULL)
	{
		return;
	}

	if (IsKeyDown(KEY_RIGHT)) mapInfo->position.x -= 5.0f;
    if (IsKeyDown(KEY_LEFT)) mapInfo->position.x += 5.0f;
    if (IsKeyDown(KEY_UP)) mapInfo->position.y += 5.0f;
    if (IsKeyDown(KEY_DOWN)) mapInfo->position.y -= 5.0f;
	SetOffset(mapInfo);
}

void Map2D_HandleMouseInput(MapInfo* mapInfo, MouseInfo* mouseInfo, MonitorSettings monitorSettings)
{
	if(mapInfo == NULL || mouseInfo == NULL)
	{
		return;
	}
	
	int mouseX = GetMouseX();
	if (mouseX < 20) mapInfo->position.x += 4.0f;
    if (mouseX < 5) mapInfo->position.x += 12.0f;
    if (mouseX > monitorSettings.monitorWidth-20) mapInfo->position.x -= 4.0f;
    if (mouseX > monitorSettings.monitorWidth-5) mapInfo->position.x -= 12.0f;

	int mouseY = GetMouseY();
	if (mouseY < 20) mapInfo->position.y += 4.0f;
    if (mouseY < 5) mapInfo->position.y += 12.0f;
    if (mouseY > monitorSettings.monitorHeight-40) mapInfo->position.y -= 4.0f;
    if (mouseY > monitorSettings.monitorHeight-25) mapInfo->position.y -= 12.0f;
	SetOffset(mapInfo);

	mouseInfo->currentPosition.x = (float)mouseX;
	mouseInfo->currentPosition.y = (float)mouseY;

	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
		mouseInfo->isSelecting = false;
		if(mouseInfo->isdragging){

		} else {
			mouseInfo->isdragging = true;
			mouseInfo->startPosition.x = (float)mouseX;
			mouseInfo->startPosition.y = (float)mouseY;
		}
	} else{
		if(mouseInfo->isdragging){
			// Done dragging => select units
			mouseInfo->isSelecting = true;
		}
		mouseInfo->isdragging = false;
	}

	mouseInfo->giveNewTarget = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
}

void Map2D_CheckBoundaries(MapInfo* mapInfo, Boundaries boundaries)
{
	if(mapInfo == NULL)
	{
		return;
	}

	if(mapInfo->position.y > boundaries.upperBoundary) mapInfo->position.y = boundaries.upperBoundary;
    if(mapInfo->position.y < boundaries.lowerBoundary) mapInfo->position.y = boundaries.lowerBoundary;
    if(mapInfo->position.x > boundaries.leftBoundary) mapInfo->position.x = boundaries.leftBoundary;
    if(mapInfo->position.x < boundaries.rightBoundary) mapInfo->position.x = boundaries.rightBoundary;
}

Rectangle Map2D_GetSelectionRectangle(MouseInfo* mouseInfo, Camera2D cam){
	float startX = mouseInfo->worldStartPosition.x;
	float startY = mouseInfo->worldStartPosition.y;
	float currentX = mouseInfo->worldCurrentPosition.x;
	float currentY = mouseInfo->worldCurrentPosition.y;

	float rectX;
	float rectY;
	float rectWidth;
	float rectHeight;

	if(currentX < startX){
		//Dragging to the left
		rectX = currentX;
		rectWidth = startX-currentX;
	} else{
		//Dragging to the right
		rectX = startX;
		rectWidth = currentX-startX;
	}

	if(currentY < startY){
		//Dragging to the top
		rectY = currentY;
		rectHeight = startY-currentY;
	} else{
		//Dragging to the bottom
		rectY = startY;
		rectHeight = currentY-startY;
	}

	Rectangle selectionRectangle;
	selectionRectangle.x = rectX;
	selectionRectangle.y = rectY;
	selectionRectangle.width = rectWidth;
	selectionRectangle.height = rectHeight;

	return selectionRectangle;
}
