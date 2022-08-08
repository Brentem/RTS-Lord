#include "../include/Debug.h"
#include "../include/2DMap.h"
#include "raylib.h"

#define DEBUG_FONT_SIZE 12

void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    MapInfo mapinfo,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight,
    MiniMapInfo miniMapInfo
    )
    {

    // DrawText(TextFormat("miniMapInfo.position.x = %01f", miniMapInfo.position.x), 10-viewportWidth/2 , 10-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // DrawText(TextFormat("miniMapInfo.position.y = %01f", miniMapInfo.position.y), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // DrawText(TextFormat("miniMapInfo.isActive = %01i", miniMapInfo.isActive), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.currentPosition.x = %01f", mouseinfo.currentPosition.x), 10-viewportWidth/2 , 10-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.currentPosition.y = %01f", mouseinfo.currentPosition.y), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // if(mouseinfo.isdragging){
    //     DrawText(TextFormat("mouseinfo.isdragging = true"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // } else{
    //     DrawText(TextFormat("mouseinfo.isdragging = false"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // }
    DrawText(TextFormat("mouseinfo.worldCurrentPosition X: %05f", mouseinfo.worldCurrentPosition.x), 10-viewportWidth/2 , 30-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.worldCurrentPosition Y: %05f", mouseinfo.worldCurrentPosition.y), 10-viewportWidth/2 , 40-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // DrawText(TextFormat("mouseinfo.worldStartPosition X: %05f", mouseinfo.worldStartPosition.x), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // DrawText(TextFormat("mouseinfo.worldStartPositionY: %05f", mouseinfo.worldStartPosition.y), 10-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mapinfo pos.x: %01f pos.y %01f", mapinfo.position.x, mapinfo.position.y), 600-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mapinfo off.x: %01f off.y %01f", mapinfo.offSet.x, mapinfo.offSet.y), 600-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    DrawText(TextFormat("camera.offset.x: %01f offset.y %01f", cam.offset.x, cam.offset.y), 600-viewportWidth/2, 80-viewportHeight/2, DEBUG_FONT_SIZE, WHITE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText(TextFormat("expected camera.offset.x: %d expected offset.y %d", screenWidth/2, screenHeight/2), 600-viewportWidth/2, 90-viewportHeight/2, DEBUG_FONT_SIZE, WHITE);
}