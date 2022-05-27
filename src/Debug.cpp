#include "../include/Debug.h"
#include "../include/2DMap.h"
#include "raylib.h"

#define DEBUG_FONT_SIZE 12

void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    Vector2 characterPosition, 
    Vector2 worldStartPosition,
    Vector2 worldCurrentPosition,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight
    )
    {
    DrawText(TextFormat("Character pos.x: %01f pos.y %01f", characterPosition.x, characterPosition.y), 10-viewportWidth/2 , 10-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    if(mouseinfo.isdragging){
        DrawText(TextFormat("mouseinfo.isdragging = true"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    } else{
        DrawText(TextFormat("mouseinfo.isdragging = false"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    }
    DrawText(TextFormat("worldCurrentPosition X: %05f", worldCurrentPosition.x), 10-viewportWidth/2 , 30-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("worldCurrentPosition Y: %05f", worldCurrentPosition.y), 10-viewportWidth/2 , 40-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("worldStartPosition X: %05f", worldStartPosition.x), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("worldStartPosition Y: %05f", worldStartPosition.y), 10-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

}