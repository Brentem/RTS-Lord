#ifndef TWODMAP_H
#define TWODMAP_H

#include "raylib.h"

#include "Types.h"
#include "MiniMap.h"

MapInfo Map2D_Init(const char *mapLayoutFileName, int cellSize);

Texture2D Map2DGetBackground(MapInfo info, const char *mapLayoutFileName, const char *tileSpritesheetFileName);

Boundaries Map2D_GetBoundaries(MapInfo info, MonitorSettings setting, float zoomFactor);

void Map2D_HandleKeyboardInput(MapInfo* info);

void Map2D_HandleMouseInput(MapInfo* info, MouseInfo* mouseinfo, MonitorSettings setting, MiniMap* miniMap, Camera2D camera);

bool Map2D_IsMouseOverMiniMap(Vector2 worldCurrentPosition, MiniMap* miniMap,  Camera2D camera);

void Map2D_CheckBoundaries(MapInfo* info, Boundaries boundaries);

Rectangle Map2D_GetSelectionRectangle(MouseInfo* mouseinfo, Camera2D cam);

// MiniMapInfo Map2D_MiniMap_Init(Texture2D background, int width, int height, int padding, Camera2D camera, MonitorSettings monitorSettings);

// void DrawMiniMap(MonitorSettings monitorSettings, MiniMapInfo miniMapInfo, MapInfo mapInfo, Scene& scene);

#endif