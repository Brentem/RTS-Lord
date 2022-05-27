#ifndef DEBUG_H
#define DEBUG_H

#include "../include/2DMap.h"
#include "../include/CharacterTest.h"

#include "raylib.h"


void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    MapInfo mapinfo,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight,
    Character* character
    );

#endif