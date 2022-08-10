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

void Map2D_HandleMouseInput(MapInfo* mapInfo, MouseInfo* mouseInfo, MonitorSettings monitorSettings, MiniMapInfo* miniMapInfo, Camera2D camera)
{
	if(mapInfo == NULL || mouseInfo == NULL)
	{
		return;
	}

	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	Vector2 currentPosition;
	currentPosition.x = (float)mouseX;
	currentPosition.y = (float)mouseY;
	mouseInfo->currentPosition = currentPosition;

	Vector2 worldCurrentPosition = GetScreenToWorld2D(currentPosition, camera); 
	mouseInfo->worldCurrentPosition = worldCurrentPosition;

	if(!(miniMapInfo->isActive && IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
		miniMapInfo->isActive = Map2D_IsMouseOverMiniMap(worldCurrentPosition, miniMapInfo, camera);
	}

	if(miniMapInfo->isActive){
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){

			// Position on minimap
			int posXOnMinimap = miniMapInfo->position.x + miniMapInfo->miniMapOffSet.x - worldCurrentPosition.x;
			int posYOnMinimap = miniMapInfo->position.y + miniMapInfo->miniMapOffSet.y- worldCurrentPosition.y;

			// Don't navigate out the minimap
			if(posXOnMinimap > miniMapInfo->widgetBoundaries.leftBoundary) posXOnMinimap = miniMapInfo->widgetBoundaries.leftBoundary;
			if(posXOnMinimap < miniMapInfo->widgetBoundaries.rightBoundary) posXOnMinimap = miniMapInfo->widgetBoundaries.rightBoundary;
			if(posYOnMinimap > miniMapInfo->widgetBoundaries.upperBoundary) posYOnMinimap = miniMapInfo->widgetBoundaries.upperBoundary;
			if(posYOnMinimap < miniMapInfo->widgetBoundaries.lowerBoundary) posYOnMinimap = miniMapInfo->widgetBoundaries.lowerBoundary;

			float posXOnMap = posXOnMinimap/miniMapInfo->zoomFactor;
			float posYOnMap = posYOnMinimap/miniMapInfo->zoomFactor;

			mapInfo->position.x = posXOnMap;
			mapInfo->position.y = posYOnMap;
			SetOffset(mapInfo);
		}

		return;
	} else{
		if (mouseX < 20) mapInfo->position.x += 4.0f;
    	if (mouseX < 5) mapInfo->position.x += 12.0f;
    	if (mouseX > monitorSettings.monitorWidth-20) mapInfo->position.x -= 4.0f;
    	if (mouseX > monitorSettings.monitorWidth-5) mapInfo->position.x -= 12.0f;

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
	
	mouseInfo->worldStartPosition = GetScreenToWorld2D(mouseInfo->startPosition, camera); 

}

bool Map2D_IsMouseOverMiniMap(Vector2 worldCurrentPosition, MiniMapInfo* miniMapInfo, Camera2D camera)
{

	if(worldCurrentPosition.x > miniMapInfo->position.x && 
		worldCurrentPosition.x < miniMapInfo->position.x + miniMapInfo->width &&
		worldCurrentPosition.y > miniMapInfo->position.y &&
		worldCurrentPosition.y < miniMapInfo->position.y + miniMapInfo->height
	)
	{
		return true;
	} else{
		return false;
	}
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

MiniMapInfo Map2D_MiniMap_Init(Texture2D background, int width, int height, int padding, Camera2D camera, MonitorSettings monitorSettings){
	MiniMapInfo miniMapInfo;
	miniMapInfo.width = width;
	miniMapInfo.height = height;
	miniMapInfo.padding = padding;

	// Calculate position on screen, remember camera is centered
	miniMapInfo.position.x = 10.0f - ((camera.offset.x/camera.zoom) + 0.05f);
	miniMapInfo.position.y = 10.0f - ((camera.offset.y/camera.zoom) + 0.05f);

	// Create minimap background
	int maxWidth = width - (padding * 2);
	if(maxWidth<0) maxWidth = 1;
	int maxHeight = height - (padding * 2);
	if(maxHeight<0) maxHeight = 1;
	
	Image backgroundImage = LoadImageFromTexture(background);
    float zoomX = (float)backgroundImage.width/maxWidth;
    float zoomY =  (float)backgroundImage.height/maxHeight;
    float zoom = (zoomY > zoomX) ? zoomY : zoomX;

	maxWidth = backgroundImage.width/zoom;
	maxHeight = backgroundImage.height/zoom;

	ImageResize(&backgroundImage, maxWidth, maxHeight);
	miniMapInfo.miniMapBackground  = LoadTextureFromImage(backgroundImage);
	UnloadImage(backgroundImage);

	// Calculate miniMapOffSet
	Vector2 miniMapOffSet;
	miniMapOffSet.x = (float)width - ((float)padding * 2) - (float)maxWidth;
	miniMapOffSet.y = (float)height - ((float)padding * 2) - (float)maxHeight;
	if(miniMapOffSet.x > 0) miniMapOffSet.x = miniMapOffSet.x/2;
	if(miniMapOffSet.y > 0) miniMapOffSet.y = miniMapOffSet.y/2;
	miniMapInfo.miniMapOffSet = miniMapOffSet;
	miniMapInfo.zoomFactor = 1/zoom;
	miniMapInfo.miniMapWidgetWidth = (int)monitorSettings.monitorWidth*miniMapInfo.zoomFactor/camera.zoom + 1.0f;
	miniMapInfo.miniMapWidgetHeight = (int)monitorSettings.monitorHeight*miniMapInfo.zoomFactor/camera.zoom + 1.0f;

	Boundaries widgetBoundaries;
	widgetBoundaries.leftBoundary = (int)(-miniMapInfo.miniMapWidgetWidth/2);
	widgetBoundaries.rightBoundary = (int)(miniMapInfo.miniMapWidgetWidth/2 - miniMapInfo.width + (miniMapInfo.miniMapOffSet.x *2))+4;
	widgetBoundaries.upperBoundary = (int)(-miniMapInfo.miniMapWidgetHeight/2);
	widgetBoundaries.lowerBoundary = (int)(miniMapInfo.miniMapWidgetHeight/2 - miniMapInfo.height + (miniMapInfo.miniMapOffSet.y *2))+4;
	miniMapInfo.widgetBoundaries = widgetBoundaries;

	return miniMapInfo;
}

void DrawMiniMap(MonitorSettings monitorSettings, MiniMapInfo miniMapInfo, MapInfo mapInfo, Scene& scene){
	DrawRectangle(miniMapInfo.position.x , miniMapInfo.position.y, miniMapInfo.width, miniMapInfo.height, BLACK );
	int posX = miniMapInfo.position.x + miniMapInfo.padding + miniMapInfo.miniMapOffSet.x;
	int posY = miniMapInfo.position.y + miniMapInfo.padding + miniMapInfo.miniMapOffSet.y;
	DrawTexture(miniMapInfo.miniMapBackground, posX, posY, WHITE);

	for(EntityID ent: SceneView<EntityPosition, Texture2D, EntitySize, bool>(scene))
    {
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);

        Vector2 characterPosition = entityPosition->currentPosition;
		Vector2 characterPositionOnMinimap;
		characterPositionOnMinimap.x = miniMapInfo.position.x + miniMapInfo.padding + miniMapInfo.width/2 + characterPosition.x*miniMapInfo.zoomFactor;
		characterPositionOnMinimap.y = miniMapInfo.position.y + miniMapInfo.padding + miniMapInfo.height/2 + characterPosition.y*miniMapInfo.zoomFactor;
		DrawCircle((int)characterPositionOnMinimap.x, (int)characterPositionOnMinimap.y, 32*miniMapInfo.zoomFactor, BLUE);
    }

	// Draw current mapposition => widget???
	// Widget centered on minimap
	int posXScreen = posX + (int)miniMapInfo.miniMapBackground.width/2 - (int)miniMapInfo.miniMapWidgetWidth/2;
	int posYScreen = posY + (int)miniMapInfo.miniMapBackground.height/2 - (int)miniMapInfo.miniMapWidgetHeight/2;
	// Process offset of background
	posXScreen = posXScreen - (int)mapInfo.offSet.x*miniMapInfo.zoomFactor;
	posYScreen = posYScreen - (int)mapInfo.offSet.y*miniMapInfo.zoomFactor;
	DrawRectangleLines(posXScreen, posYScreen, miniMapInfo.miniMapWidgetWidth, (int)miniMapInfo.miniMapWidgetHeight, WHITE);

	if( miniMapInfo.isActive){
		DrawRectangleLines(miniMapInfo.position.x , miniMapInfo.position.y, miniMapInfo.width, miniMapInfo.height, WHITE );
	}
}
