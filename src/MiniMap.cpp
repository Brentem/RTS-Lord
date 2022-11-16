#include "../include/MiniMap.h"

MiniMap::MiniMap(Texture2D texture, Camera2D cam, MonitorSettings monitorSettings,
 int width, int height, int padding, int marginX, int marginY)
: HudElement(texture, cam, width, height, marginX, marginY)
{
    this->padding = padding;
    this->width = width;
    this->height = height;
    isActive = false;

    // Create minimap background
	int maxWidth = width - (padding * 2);
	if(maxWidth<0) maxWidth = 1;
	int maxHeight = height - (padding * 2);
	if(maxHeight<0) maxHeight = 1;
	
	Image backgroundImage = LoadImageFromTexture(texture);
    float zoomX = (float)backgroundImage.width/maxWidth;
    float zoomY =  (float)backgroundImage.height/maxHeight;
    float zoom = (zoomY > zoomX) ? zoomY : zoomX;

    maxWidth = backgroundImage.width/zoom;
	maxHeight = backgroundImage.height/zoom;

	ImageResize(&backgroundImage, maxWidth, maxHeight);
	this->texture = LoadTextureFromImage(backgroundImage);
	UnloadImage(backgroundImage);

    // Calculate miniMapOffSet
	Vector2 offSet;
	offSet.x = (float)width - ((float)padding * 2) - (float)maxWidth;
	offSet.y = (float)height - ((float)padding * 2) - (float)maxHeight;
	if(offSet.x > 0) offSet.x = offSet.x/2;
	if(offSet.y > 0) offSet.y = offSet.y/2;
	miniMapOffSet = offSet;
	zoomFactor = 1/zoom;
	miniMapWidgetWidth = (int)monitorSettings.monitorWidth*zoomFactor/cam.zoom + 1.0f;
	miniMapWidgetHeight = (int)monitorSettings.monitorHeight*zoomFactor/cam.zoom + 1.0f;

    widgetBoundaries.leftBoundary = (int)(-miniMapWidgetWidth/2);
    widgetBoundaries.rightBoundary = (int)(miniMapWidgetWidth/2 - width + (miniMapOffSet.x *2))+4;
	widgetBoundaries.upperBoundary = (int)(-miniMapWidgetHeight/2);
	widgetBoundaries.lowerBoundary = (int)(miniMapWidgetHeight/2 - height + (miniMapOffSet.y *2))+4;

    // Calculate background offset
    offsetPosX = position.x + padding + miniMapOffSet.x;
    offsetPosY = position.y + padding + miniMapOffSet.y;
}

void MiniMap::Update(MapInfo info)
{
    // Draw current mapposition => widget???
	// Widget centered on minimap
    posXScreen = offsetPosX + (int)HudElement::texture.width/2 - (int)miniMapWidgetWidth/2;
    posYScreen = offsetPosY + (int)HudElement::texture.height/2 - (int)miniMapWidgetHeight/2;

    // Process offset of background
	posXScreen = posXScreen - (int)info.offSet.x*zoomFactor;
	posYScreen = posYScreen - (int)info.offSet.y*zoomFactor;
}

void MiniMap::Draw()
{
    DrawRectangle(position.x, position.y, width, height, BLACK);
    DrawTexture(texture, offsetPosX, offsetPosY, WHITE);

    // White rectangle
    if(isActive){
		DrawRectangleLines(position.x , position.y, width, height, WHITE);
	}
}