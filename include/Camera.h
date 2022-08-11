#ifndef CAMERA_H
#define CAMERA_H

extern "C"
{
    #include "raylib.h"
}


Camera2D Camera_Init(int monitorWidth, int monitorHeight, int viewportWidth, int viewportHeight);

#endif