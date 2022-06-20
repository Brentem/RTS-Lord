#ifndef TWODMAP_H
#define TWODMAP_H

#include "raylib.h"

#include "Types.h"

MapInfo Map2D_Init(int rowCount, int columnCount, int cellSize);

Texture2D Map2DGetBackground(MapInfo info);

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting, float zoomFactor);

void Map2D_HandleKeyboardInput(MapInfo* info);

void Map2D_HandleMouseInput(MapInfo* info, MouseInfo* mouseinfo, MonitorSettings setting);

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries);

Rectangle Map2D_GetSelectionRectangle(MouseInfo* mouseinfo, Camera2D cam);

#endif