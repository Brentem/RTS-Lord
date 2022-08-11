#pragma once

#include "SceneView.h"
#include "Types.h"
#include "MiniMap.h"

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void RenderSystem(Scene& scene, MapInfo mapInfo);
void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap);
