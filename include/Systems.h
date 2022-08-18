#pragma once

#include "SceneView.h"
#include "Types.h"
#include "MiniMap.h"

#include <vector>

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, std::vector<std::vector<Tile>> grid);
void RenderSystem(Scene& scene, MapInfo mapInfo);
void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap);
