#include "../include/2DMap.h"

#include <stdlib.h>

MapInfo Map2D_Init(const char *mapLayoutFileName, int cellSize)
{
	Image mapSource = LoadImage(mapLayoutFileName); // "assets/map1.png"

	MapInfo mapInfo;
	mapInfo.rowCount = mapSource.height;
	mapInfo.columnCount = mapSource.width;
	mapInfo.cellSize = cellSize;
	mapInfo.mapWidth = mapInfo.columnCount * cellSize;
	mapInfo.mapHeight = mapInfo.rowCount * cellSize;
	mapInfo.position = (Vector2){(((float)mapInfo.mapWidth /2) *-1), (((float)mapInfo.mapHeight/2) *-1)};
	mapInfo.offSet = {0.0f, 0.0f};

	UnloadImage(mapSource);

	return mapInfo;
}

Texture2D Map2DGetBackground(MapInfo mapInfo,const char *mapLayoutFileName, const char *tileSpritesheetFileName){

	// Texture2D spriteSheet = LoadTexture("../assets/spritesheet.png"); 
	Image spriteSheet = LoadImage(tileSpritesheetFileName); // "assets/spritesheet.png"
	Image mapSource = LoadImage(mapLayoutFileName); // "assets/map1.png"

	Rectangle tileDirt = { 0.0f, 0.0f, 32.0f, 32.0f};
	Rectangle tileGrass1 = { 32.0f, 0.0f, 32.0f, 32.0f};
	Rectangle tileGrass2 = { 0.0f, 32.0f, 32.0f, 32.0f};
	Rectangle tileGrass3 = { 32.0f, 32.0f, 32.0f, 32.0f};

	Image imageBackground = GenImageColor(mapInfo.mapWidth, mapInfo.mapHeight, WHITE);
	for (int x = 0; x < mapInfo.columnCount; x++){
		for (int y = 0; y < mapInfo.rowCount; y++){
			Color color = GetImageColor(mapSource, x, y); 
			// r    g    b
			// 122  74	 52
			// 57	181	 74	Donkergroen
			// 0	166	 81	Groen
			// 141	198	 63	Lichtgroen
			Rectangle spriteRectangle;
			if(color.r == 122){
				spriteRectangle = tileDirt;
			} else if(color.r == 57){
				spriteRectangle = tileGrass1;
			} else if(color.r == 0){
				spriteRectangle = tileGrass2;
			} else if(color.r == 141){
				spriteRectangle = tileGrass3;
			}
			ImageDraw(&imageBackground, spriteSheet, spriteRectangle,  (Rectangle) { (float)x*mapInfo.cellSize, (float)y*mapInfo.cellSize, (float)mapInfo.cellSize, (float)mapInfo.cellSize }, WHITE);
		}
	}

	ImageDrawRectangleLines(&imageBackground, (Rectangle) { 0, 0, (float)mapInfo.mapWidth, (float)mapInfo.mapHeight }, 4, RED);

	Texture2D background = LoadTextureFromImage(imageBackground);
	UnloadImage(imageBackground);
	UnloadImage(spriteSheet);
	UnloadImage(mapSource);

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

void SetOffset(MapInfo* mapInfo){
	if(mapInfo == NULL)
	{
		return;
	}

	mapInfo->offSet.x = (float)mapInfo->mapWidth/2 + mapInfo->position.x;
	mapInfo->offSet.y = (float)mapInfo->mapHeight/2 + mapInfo->position.y;
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

void Map2D_UpdateMouseInfo(MouseInfo* mouseInfo, MapInfo* mapInfo){
	
	SetOffset(mapInfo);
	
	mouseInfo->currentPositionOnMap = GetPositionOnMap(mouseInfo->worldCurrentPosition, mapInfo->offSet, mapInfo->mapWidth, mapInfo->mapHeight);
	mouseInfo->gridCell = GetGridPosition( mouseInfo->currentPositionOnMap, mapInfo->cellSize);
}

Vector2 GetPositionOnMap(Vector2 worldPosition, Vector2 mapOffset, int mapWidth, int mapHeight){
	Vector2 positionOnMap;
	positionOnMap.x = worldPosition.x - mapOffset.x + (float)mapWidth/2;
	positionOnMap.y = worldPosition.y - mapOffset.y + (float)mapHeight/2;
	return positionOnMap;
}

Vector2 GetPositionOnMap(Vector2 worldPosition, int mapWidth, int mapHeight){
	Vector2 positionOnMap;
	positionOnMap.x = worldPosition.x + (float)mapWidth/2;
	positionOnMap.y = worldPosition.y + (float)mapHeight/2;
	return positionOnMap;
}

Pair GetGridPosition(Vector2 positionOnMap, int cellSize){
	return Pair(positionOnMap.x/cellSize, positionOnMap.y/cellSize);
}

Vector2 GetPositionOnMap(Pair gridPosition, int cellSize){
	Vector2 positionOnMap;
	positionOnMap.x = gridPosition.first*cellSize;
	positionOnMap.y = gridPosition.second*cellSize;
	return positionOnMap;
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

void DrawMouseGrid(int mouseGridSizeX, int mouseGridSizeY, MouseInfo mouseInfo, MapInfo mapInfo, std::vector<std::vector<Tile>> grid)
{
	
	Vector2 virtualPositionOnMap;
	virtualPositionOnMap.x = mouseInfo.currentPositionOnMap.x - (mapInfo.cellSize/2*(mouseGridSizeX-1));
	virtualPositionOnMap.y = mouseInfo.currentPositionOnMap.y - (mapInfo.cellSize/2*(mouseGridSizeY-1));

	int gridCellX = virtualPositionOnMap.x/mapInfo.cellSize;
	int gridCellY = virtualPositionOnMap.y/mapInfo.cellSize;

	for (int x = 0; x < mouseGridSizeX; x++)
	{
		for (int y = 0; y < mouseGridSizeY; y++)
		{
			int virtualGridCellX = gridCellX + x;
			int virtualGridCellY = gridCellY + y;
			if(virtualGridCellX < 0 || virtualGridCellX >= mapInfo.columnCount || virtualGridCellY < 0 || virtualGridCellY >= mapInfo.rowCount){
				continue;
			}

			Color color = WHITE;
			Tile tile = grid[virtualGridCellX][virtualGridCellY];
			if(!tile.isWalkable){
				color = RED;
			}
			
			DrawRectangle(
				virtualGridCellX*mapInfo.cellSize - mapInfo.mapWidth/2 + mapInfo.offSet.x, 
            	virtualGridCellY*mapInfo.cellSize - mapInfo.mapHeight/2 + mapInfo.offSet.y, 
            	mapInfo.cellSize,
            	mapInfo.cellSize, 
            	Fade(color, 0.2f));
			DrawRectangleLines(
				virtualGridCellX*mapInfo.cellSize - mapInfo.mapWidth/2 + mapInfo.offSet.x, 
            	virtualGridCellY*mapInfo.cellSize - mapInfo.mapHeight/2 + mapInfo.offSet.y, 
            	mapInfo.cellSize,
            	mapInfo.cellSize, 
            	color);		
		}
	}	
}
