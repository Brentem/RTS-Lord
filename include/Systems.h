#pragma once

#include "Types.h"
#include "MiniMap.h"
#include "Scene.h"

#include <vector>

void InitSystem();
void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, std::vector<std::vector<Tile>>& grid, Camera2D camera, float deltaT);
void RenderSystem(Scene& scene, MapInfo mapInfo, float deltaT);
void ProcessAnimation(Animation& animation, EntitySize& size, float deltaT, Vector2 characterPositionOnMap);
void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap);
