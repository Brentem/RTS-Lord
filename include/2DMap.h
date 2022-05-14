#ifndef TWODMAP_H
#define TWODMAP_H

#include "raylib.h"

#include "Types.h"

typedef struct 
{
    int mapWidth;
    int mapHeight;
    Vector2 position;
}MapInfo;

typedef struct
{
    int upperBoundary;
    int lowerBoundary;
    int leftBoundary;
    int rightBoundary;
} Boundaries;


MapInfo Map2D_Init(int mapWidth, int mapHeight);

Texture2D Map2DGetBackground(MapInfo info);

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting);

void Map2D_HandleKeyboardInput(MapInfo* info);

void Map2D_HandleMouseInput(MapInfo* info, MonitorSettings setting);

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries);

#endif