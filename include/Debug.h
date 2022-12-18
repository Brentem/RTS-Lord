#ifndef DEBUG_H
#define DEBUG_H

#include "Types.h"
#include "../include/Scene.h"

extern "C"
{
    #include "raylib.h"
}


void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    MapInfo mapinfo,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight,
    Scene& scene
    //MiniMapInfo miniMapInfo
    );

void DrawBasesDebugInfo(int& posy, Scene& scene, MapInfo mapinfo, int viewportWidth, int viewportHeight);

void DrawSelectedUnitDebugInfo(int& posy, Scene& scene, MapInfo mapinfo, int viewportWidth, int viewportHeight);

#endif