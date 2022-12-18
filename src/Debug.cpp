#include "../include/Debug.h"
#include "../include/2DMap.h"

extern "C"
{
    #include "raylib.h"
}

#define DEBUG_FONT_SIZE 12

void Debug_DrawDebugInfo(
    MouseInfo mouseinfo, 
    MapInfo mapinfo,
    Camera2D cam, 
    int viewportWidth, 
    int viewportHeight,
    Scene& scene
    //MiniMapInfo miniMapInfo
    )
    {

    int posy = 10;
    DrawBasesDebugInfo(posy, scene, mapinfo, viewportWidth, viewportHeight);
    DrawSelectedUnitDebugInfo(posy, scene, mapinfo, viewportWidth, viewportHeight);
}

void DrawSelectedUnitDebugInfo(int& posy, Scene& scene, MapInfo mapinfo, int viewportWidth, int viewportHeight){
    auto unitView = scene.registry.view<EntityPosition, IsSelected>();
    for(auto entity : unitView)
    {
        IsSelected& isSelected = unitView.get<IsSelected>(entity);
        EntityPosition& position = unitView.get<EntityPosition>(entity);
        if(isSelected.Value)
        {
            DrawText(TextFormat("position.currentPosition.x = %01f", position.currentPosition.x), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("position.currentPosition.y = %01f", position.currentPosition.y), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;

            Vector2 positionOnMap;
	        positionOnMap.x = position.currentPosition.x + (float)mapinfo.mapWidth/2;
	        positionOnMap.y = position.currentPosition.y + (float)mapinfo.mapHeight/2;
            DrawText(TextFormat("positionOnMap.x = %01f", positionOnMap.x), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("positionOnMap.y = %01f", positionOnMap.y), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;

            Pair gridPosition = GetGridPosition(positionOnMap, mapinfo.cellSize);
            DrawText(TextFormat("gridPosition.first = %01i", gridPosition.first), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("gridPosition.second = %01i", gridPosition.second), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 15;
        }
    }
}

void DrawBasesDebugInfo(int& posy, Scene& scene, MapInfo mapinfo, int viewportWidth, int viewportHeight){
    auto baseView = scene.registry.view<EntityPosition, EntityType>();
    for(auto entity : baseView)
    {
        EntityPosition& position = baseView.get<EntityPosition>(entity);
        EntityType& type = baseView.get<EntityType>(entity);
        if(type.Value == EntityType::Building)
        {
            DrawText(TextFormat("position.currentPosition.x = %01f", position.currentPosition.x), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("position.currentPosition.y = %01f", position.currentPosition.y), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;

            Vector2 positionOnMap;
	        positionOnMap.x = position.currentPosition.x + (float)mapinfo.mapWidth/2;
	        positionOnMap.y = position.currentPosition.y + (float)mapinfo.mapHeight/2;
            DrawText(TextFormat("positionOnMap.x = %01f", positionOnMap.x), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("positionOnMap.y = %01f", positionOnMap.y), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;

            Pair gridPosition = GetGridPosition(positionOnMap, mapinfo.cellSize);
            DrawText(TextFormat("gridPosition.first = %01i", gridPosition.first), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 10;
            DrawText(TextFormat("gridPosition.second = %01i", gridPosition.second), 600-viewportWidth/2 , posy-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
            posy += 15;
        }
    }
}

void DrawDebugInfoSet1(MouseInfo mouseinfo, MapInfo mapinfo, Camera2D cam, int viewportWidth, int viewportHeight){
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
    
    DrawText(TextFormat("mouseinfo.gridCellX: %02i", mouseinfo.gridCell.first), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mouseinfo.gridCellY: %02i", mouseinfo.gridCell.second), 10-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    // DrawText(TextFormat("mouseinfo.currentPositionOnMap.x: %05f", mouseinfo.currentPositionOnMap.x), 10-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    // DrawText(TextFormat("mouseinfo.currentPositionOnMap.y: %05f", mouseinfo.currentPositionOnMap.y), 10-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    DrawText(TextFormat("mapinfo pos.x: %01f pos.y %01f", mapinfo.position.x, mapinfo.position.y), 600-viewportWidth/2 , 50-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);
    DrawText(TextFormat("mapinfo off.x: %01f off.y %01f", mapinfo.offSet.x, mapinfo.offSet.y), 600-viewportWidth/2 , 60-viewportHeight/2 , DEBUG_FONT_SIZE, WHITE);

    DrawText(TextFormat("camera.rotation: %01f offset.y %01f", cam.rotation, cam.offset.y), 600-viewportWidth/2, 80-viewportHeight/2, DEBUG_FONT_SIZE, WHITE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText(TextFormat("expected camera.offset.x: %d expected offset.y %d", screenWidth/2, screenHeight/2), 600-viewportWidth/2, 90-viewportHeight/2, DEBUG_FONT_SIZE, WHITE);
}