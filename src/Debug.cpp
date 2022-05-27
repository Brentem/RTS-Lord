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
    Character* character
    )
    {

    DrawText(TextFormat("mouseinfo.currentPosition.x = %01f", mouseinfo.currentPosition.x), 10-viewportWidth/2 , 10-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    if(mouseinfo.isdragging){
        DrawText(TextFormat("mouseinfo.isdragging = true"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    } else{
        DrawText(TextFormat("mouseinfo.isdragging = false"), 10-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    }
    DrawText(TextFormat("mouseinfo.worldCurrentPosition X: %05f", mouseinfo.worldCurrentPosition.x), 10-viewportWidth/2 , 30-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.worldCurrentPosition Y: %05f", mouseinfo.worldCurrentPosition.y), 10-viewportWidth/2 , 40-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.worldStartPosition X: %05f", mouseinfo.worldStartPosition.x), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.worldStartPositionY: %05f", mouseinfo.worldStartPosition.y), 10-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    DrawText(TextFormat("Character pos.x: %01f pos.y %01f", character->getPosition().x, character->getPosition().y), 600-viewportWidth/2 , 10-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("Character col.x: %01f pos.y %01f", character->getCollisionRect().x, character->getCollisionRect().y), 600-viewportWidth/2 , 20-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("Character col.width: %01f height %01f", character->getCollisionRect().width, character->getCollisionRect().height), 600-viewportWidth/2 , 30-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    DrawText(TextFormat("mapinfo pos.x: %01f pos.y %01f", mapinfo.position.x, mapinfo.position.y), 600-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mapinfo off.x: %01f off.y %01f", mapinfo.offSet.x, mapinfo.offSet.y), 600-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
}