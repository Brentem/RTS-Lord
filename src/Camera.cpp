#include "../include/Camera.h"

Camera2D Camera_Init(int monitorWidth, int monitorHeight, int viewportWidth, int viewportHeight)
{
    Camera2D cam;

    float zoomX = (float)monitorWidth/viewportWidth;
    float zoomY = (float)monitorHeight/viewportHeight;
    float zoom = (zoomY < zoomX) ? zoomY : zoomX;

    cam.zoom = zoom;
    // center the camera on the middle of the screen
    cam.offset = (Vector2){monitorWidth / 2, monitorHeight / 2};

    return cam;
}