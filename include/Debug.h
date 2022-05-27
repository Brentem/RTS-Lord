#ifndef DEBUG_H
#define DEBUG_H

#include "../include/2DMap.h"

#include "raylib.h"


void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    Vector2 characterPosition, 
    Vector2 worldStartPosition,
    Vector2 worldCurrentPosition,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight
    );

#endif