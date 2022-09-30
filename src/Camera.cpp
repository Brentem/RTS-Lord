#include "../include/Camera.h"

Camera2D Camera_Init(int monitorWidth, int monitorHeight, int viewportWidth, int viewportHeight)
{
    Camera2D cam;

    float zoomX = (float)monitorWidth/viewportWidth;
    float zoomY = (float)monitorHeight/viewportHeight;
    float zoom = (zoomY < zoomX) ? zoomY : zoomX;

    cam.zoom = zoom;
    cam.rotation = 0.0f;
    // center the camera on the middle of the screen
    cam.offset = (Vector2){(float)monitorWidth / 2, (float)monitorHeight / 2};
    cam.target = (Vector2) {0.0f, 0.0f};

    return cam;
}