#pragma once

#include "Types.h"

#define UI_WIDTH 1070
#define UI_LENGTH 800

#define UI_POSY_MARGIN -218.0f

UI UI_Init(int width, int height);
void UI_Draw(UI ui, Camera2D camera);