#pragma once

#include "Types.h"
#include "MiniMap.h"
#include "Scene.h"

#include <vector>

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, std::vector<std::vector<Tile>>& grid, Camera2D camera);
void RenderSystem(Scene& scene, MapInfo mapInfo);
void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap);
