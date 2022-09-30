#ifndef TWODMAP_H
#define TWODMAP_H

extern "C"
{
    #include "raylib.h"
}

#include "Types.h"
#include "MiniMap.h"

MapInfo Map2D_Init(const char *mapLayoutFileName, int cellSize);

Texture2D Map2DGetBackground(MapInfo info, const char *mapLayoutFileName, const char *tileSpritesheetFileName);

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting, float zoomFactor);

void Map2D_HandleKeyboardInput(MapInfo* info);

void Map2D_HandleMouseInput(MapInfo* info, MouseInfo* mouseinfo, MonitorSettings setting, MiniMap* miniMap, Camera2D camera);

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries);

void Map2D_UpdateMouseInfo(MouseInfo* mouseInfo, MapInfo* mapInfo);

Rectangle Map2D_GetSelectionRectangle(MouseInfo* mouseinfo, Camera2D cam);

void DrawMouseGrid(int mouseGridSizeX, int mouseGridSizeY, MouseInfo mouseInfo, MapInfo mapInfo, std::vector<std::vector<Tile>> grid);

#endif