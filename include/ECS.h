#pragma once

#include <raylib.h>

/*
TODO:(Brent)
* Think about where to save positions in main or in rendersystem?
* Look up how to use hash tables
* Change DrawEntities Method
*/

#include "2DMap.h"

void DrawEntities(Texture2D characterTexture, MapInfo info);