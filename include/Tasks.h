#pragma once
 
 #include "Scene.h"
 #include "Types.h"
 #include "Pathfinding.h"

void GatheringTask(Scene &scene, MouseInfo mouseInfo, MapInfo mapInfo);
void CheckResources(Scene& scene, MapInfo mapInfo, MouseInfo& mouseInfo);
void CheckBases(Scene& scene, MapInfo& mapInfo, MouseInfo& mouseInfo, std::vector<std::vector<Tile>>& grid);
