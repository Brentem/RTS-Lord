#include "../include/Systems.h"

#include "../include/Types.h"
#include "../include/Pathfinding.h"
#include "../include/2DMap.h"
#include "../include/Tasks.h"

#include <stdio.h>

using namespace std;

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid, Camera2D camera);
Pair GetValidTargetGridCell(Pair startGridCell, Pair selectedTargetGridCell, vector<vector<Tile>>& grid);
void setTargetPosition(Scene& scene, MapInfo mapInfo, Camera2D camera);
void updatePosition(Scene& scene);

Pair getPair(Vector2 position);
float getPositionIndex(int pairIndex);

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, vector<vector<Tile>>& grid, Camera2D camera)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    checkIfSelected(scene, mouseInfo, mapInfo, selection);
    setPath(scene, mouseInfo, mapInfo, grid, camera);
    GatheringTask(scene, *mouseInfo, mapInfo);
    setTargetPosition(scene, mapInfo, camera);
    updatePosition(scene);
}

void RenderSystem(Scene& scene, MapInfo mapInfo)
{
    auto view = scene.registry.view<EntityPosition, Texture2D, EntityType>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Texture2D& texture = view.get<Texture2D>(entity);
        EntityType& type = view.get<EntityType>(entity);

        Vector2 currentPosition = entityPosition.currentPosition;
        Vector2 positionOnMap = {currentPosition.x + mapInfo.offSet.x, currentPosition.y + mapInfo.offSet.y};

        if(type.Value != EntityType::Worker)
        {
            DrawTextureV(texture, positionOnMap, WHITE);
        }
    }

    auto secondView = scene.registry.view<EntityPosition, Texture2D, EntitySize, bool>();
    for(auto entity : secondView)
    {
        EntityPosition& entityPosition = secondView.get<EntityPosition>(entity);
        Texture2D& texture = secondView.get<Texture2D>(entity);
        EntitySize& size = secondView.get<EntitySize>(entity);
        bool& isSelected = secondView.get<bool>(entity);

        Vector2 characterPosition = entityPosition.currentPosition;
        Vector2 characterPositionOnMap = {(characterPosition.x + mapInfo.offSet.x), (characterPosition.y + mapInfo.offSet.y)};
        Rectangle frameRec = { 0.0f, 0.0f, size.width, size.height };
        DrawTextureRec(texture, frameRec, characterPositionOnMap, WHITE);

        if(isSelected)
        {
            DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, size.width, size.height, RED);
        }
    }
}

void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap)
{
    if(miniMap == nullptr)
    {
        return;
    }

    auto view = scene.registry.view<EntityPosition, EntityType>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        EntityType& entityType = view.get<EntityType>(entity);

        if(entityType.Value == EntityType::Worker)
        {
            Vector2 characterPosition = entityPosition.currentPosition;
		    Vector2 characterPositionOnMinimap;
		    characterPositionOnMinimap.x = miniMap->position.x + miniMap->padding + miniMap->width/2 + characterPosition.x*miniMap->zoomFactor;
		    characterPositionOnMinimap.y = miniMap->position.y + miniMap->padding + miniMap->height/2 + characterPosition.y*miniMap->zoomFactor;
		    DrawCircle((int)characterPositionOnMinimap.x, (int)characterPositionOnMinimap.y, 32*miniMap->zoomFactor, BLUE);
        }
    }

    DrawRectangleLines(miniMap->posXScreen, miniMap->posYScreen, miniMap->miniMapWidgetWidth, miniMap->miniMapWidgetHeight, WHITE);
}

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection)
{
    if(mouseInfo->isSelecting)
    {
        Rectangle selectionRectangleOnMap = {selection.x - mapInfo.offSet.x, selection.y - mapInfo.offSet.y , selection.width, selection.height};

        auto view = scene.registry.view<EntityPosition, EntitySize, bool>();
        for(auto entity : view)
        {
            bool& isSelected = view.get<bool>(entity);
            EntityPosition& entityPosition = view.get<EntityPosition>(entity);
            EntitySize& size = view.get<EntitySize>(entity);
            
            Rectangle entityBox = {entityPosition.currentPosition.x, entityPosition.currentPosition.y,
                                    size.width, size.height};

            if(mouseInfo->selectedUnits < MAX_UNITS_SELECTED)
            {
                isSelected = CheckCollisionRecs(selectionRectangleOnMap, entityBox);
            }
            else
            {
                isSelected = false;
            }

            if(isSelected)
            {
                mouseInfo->selectedUnits++;
            }
        }

        mouseInfo->isSelecting = false;
    }
}

void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid, Camera2D camera)
{
    auto view = scene.registry.view<EntityPosition, bool, Path>();
    for(auto entity : view)
    {
        bool& isSelected = view.get<bool>(entity);
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Path& path = view.get<Path>(entity);

        if(isSelected && mouseInfo->giveNewTarget)
        {
            Vector2 currentPosition;
            currentPosition.x = entityPosition.currentPosition.x + mapInfo.offSet.x;
            currentPosition.y = entityPosition.currentPosition.y + mapInfo.offSet.y;
            Pair startGridCell = GetGridPosition(GetPositionOnMap(currentPosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, mouseInfo->gridCell, grid);
            path = GetPath(mapInfo, startGridCell, targetGridCell, grid);
        }
    }
    mouseInfo->giveNewTarget = false;
}

Pair GetValidTargetGridCell(Pair startGridCell, Pair selectedTargetGridCell, vector<vector<Tile>>& grid){
    Tile selectedTile = grid[selectedTargetGridCell.first][selectedTargetGridCell.second];
    if(selectedTile.isWalkable){
        return selectedTargetGridCell;
    }
    
    int xFactor = 0;
    if(startGridCell.first > selectedTargetGridCell.first ){
        xFactor = 1;
    } else if(startGridCell.first < selectedTargetGridCell.first){
        xFactor = -1;
    }

    int yFactor = 0;
    if(startGridCell.second > selectedTargetGridCell.second ){
        yFactor = 1;
    } else if(startGridCell.second < selectedTargetGridCell.second){
        yFactor = -1;
    }

    for (size_t i = 1; i < 4; i++)
    {
        Pair tryNewTargetGridCell;
        tryNewTargetGridCell.first = selectedTargetGridCell.first+(xFactor*i);
        tryNewTargetGridCell.second = selectedTargetGridCell.second+(yFactor*i);
        selectedTile = grid[tryNewTargetGridCell.first][tryNewTargetGridCell.second];
        if(selectedTile.isWalkable){
            return tryNewTargetGridCell;
        }
    }

    return startGridCell;
}

void setTargetPosition(Scene& scene, MapInfo mapInfo, Camera2D camera)
{
    auto view = scene.registry.view<EntityPosition, Path>();
    for(auto entity : view)
    {
        EntityPosition& position = view.get<EntityPosition>(entity);
        Path& path = view.get<Path>(entity);

        if(!(path.empty()))
        {
            Pair gridPosition = path.back();
            Vector2 positionOnMap = GetPositionOnMap(gridPosition, mapInfo.cellSize);
            Vector2 targetPosition = (Vector2){positionOnMap.x - (mapInfo.mapWidth/2),positionOnMap.y - (mapInfo.mapHeight/2)};
            position.targetPosition = targetPosition;

            if((position.currentPosition.x == position.targetPosition.x) &&
                (position.currentPosition.y == position.targetPosition.y))
            {
                path.pop_back();
            }
        }
    }
}

void updatePosition(Scene& scene)
{
    auto view = scene.registry.view<EntityPosition>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Vector2* currentPosition = &entityPosition.currentPosition;
        Vector2* targetPosition = &entityPosition.targetPosition;

        bool isMovingDiagonally = (targetPosition->x != currentPosition->x) && (targetPosition->y != currentPosition->y);
        
        // TODO should be property of the entity
        float speed = 1.0f;   
        // TODO should be property of the entity AND calculated correctly          
        float diagonalSpeed = 0.75f;    

        if(targetPosition->x > currentPosition->x){
            if(isMovingDiagonally){
                currentPosition->x += diagonalSpeed;
            } else{
                currentPosition->x += speed;
            }
            if(targetPosition->x < currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->x < currentPosition->x){
            if(isMovingDiagonally){
                currentPosition->x -= diagonalSpeed;
            } else{
                currentPosition->x -= speed;
            }
            if(targetPosition->x > currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->y > currentPosition->y){
            if(isMovingDiagonally){
                currentPosition->y += diagonalSpeed;
            } else{
                currentPosition->y += speed;
            }
            if(targetPosition->y < currentPosition->y) currentPosition->y = targetPosition->y;
        }

        if(targetPosition->y < currentPosition->y){
            if(isMovingDiagonally){
                currentPosition->y -= diagonalSpeed;
            } else{
                currentPosition->y -= speed;
            }
            if(targetPosition->y > currentPosition->y) currentPosition->y = targetPosition->y;
        }
    }
}

Pair getPair(Vector2 position)
{
    int x = 0;
    int y = 0;

    if(position.x < 0)
    {
        x = (int)((position.x / 16) / 2 - 0.5);
    }
    else
    {
        x = (int)((position.x / 16) / 2 + 0.5);
    }

    if(position.y < 0)
    {
        y = (int)((position.y / 16) / 2 - 0.5);
    }
    else
    {
        y = (int)((position.y / 16) / 2 + 0.5);
    }

    return Pair(x, y);
}

float getPositionIndex(int pairIndex)
{
    return pairIndex * 32;
}