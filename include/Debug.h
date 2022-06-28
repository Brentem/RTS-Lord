#ifndef DEBUG_H
#define DEBUG_H

#include "Types.h"

#include "raylib.h"


void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    MapInfo mapinfo,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight
    );

#endif