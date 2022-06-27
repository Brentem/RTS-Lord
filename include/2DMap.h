#ifndef TWODMAP_H
#define TWODMAP_H

#include "raylib.h"

#include "Types.h"
#include <vector>

typedef struct {
    bool isWalkable;
}Tile;

typedef struct 
{
    int columnCount;
    int rowCount;
    int cellSize;
    int mapWidth;
    int mapHeight;
    Vector2 position;
    Vector2 offSet;
    std::vector<std::vector<Tile>> tiles;
}MapInfo;

typedef struct 
{
    int screenPositionX;
    int screenPositionY;
    int width;
    int height;
    int padding;
    Vector2 miniMapOffSet;
    Texture2D miniMapBackground;
}MiniMapInfo;

typedef struct
{
    int upperBoundary;
    int lowerBoundary;
    int leftBoundary;
    int rightBoundary;
} Boundaries;

typedef struct
{
    Vector2 startPosition;
    Vector2 currentPosition;
    Vector2 worldStartPosition;
    Vector2 worldCurrentPosition;
    bool isdragging;
    bool isSelecting;
    bool giveNewTarget;
}MouseInfo;

MapInfo Map2D_Init(const char *mapLayoutFileName, int cellSize);

Texture2D Map2DGetBackground(MapInfo info, const char *mapLayoutFileName, const char *tileSpritesheetFileName);

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting, float zoomFactor);

void Map2D_HandleKeyboardInput(MapInfo* info);

void Map2D_HandleMouseInput(MapInfo* info, MouseInfo* mouseinfo, MonitorSettings setting);

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries);

Rectangle Map2D_GetSelectionRectangle(MouseInfo* mouseinfo, Camera2D cam);

MiniMapInfo Map2D_MiniMap_Init(Texture2D background, int width, int height, int padding, Camera2D camera);

void DrawMiniMap(MonitorSettings monitorSettings, MiniMapInfo miniMapInfo);

#endif