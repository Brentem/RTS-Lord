#include "../include/Systems.h"

#include "../include/Types.h"
#include "../include/Pathfinding.h"
#include "../include/2DMap.h"
#include "../include/Tasks.h"

#include <stdio.h>

using namespace std;

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setSelectedCell(Scene& scene, MouseInfo mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid);
void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid, Camera2D camera);
Pair GetValidTargetGridCell(Pair startGridCell, Pair selectedTargetGridCell, vector<vector<Tile>>& grid);
void setTargetPosition(Scene& scene, MapInfo mapInfo, Camera2D camera);
void updatePosition(Scene& scene);

Pair getPair(Vector2 position);
float getPositionIndex(int pairIndex);

// Maybe this function does too much
void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, vector<vector<Tile>>& grid, Camera2D camera)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    checkIfSelected(scene, mouseInfo, mapInfo, selection);
    setSelectedCell(scene, *mouseInfo, mapInfo, grid); // This function is important for the other functions to function properly.
    CheckResources(scene, mapInfo, *mouseInfo);
    CheckBases(scene, mapInfo, *mouseInfo, grid);
    setPath(scene, mouseInfo, mapInfo, grid, camera);
    GatheringTask(scene, *mouseInfo, mapInfo); // Probably should be changed
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

    auto secondView = scene.registry.view<EntityPosition, Texture2D, EntitySize, IsSelected>();
    for(auto entity : secondView)
    {
        EntityPosition& entityPosition = secondView.get<EntityPosition>(entity);
        Texture2D& texture = secondView.get<Texture2D>(entity);
        EntitySize& size = secondView.get<EntitySize>(entity);
        IsSelected& isSelected = secondView.get<IsSelected>(entity);

        Vector2 characterPosition = entityPosition.currentPosition;
        Vector2 characterPositionOnMap = {(characterPosition.x + mapInfo.offSet.x), (characterPosition.y + mapInfo.offSet.y)};
        Rectangle frameRec = { 0.0f, 0.0f, size.width, size.height };
        DrawTextureRec(texture, frameRec, characterPositionOnMap, WHITE);

        if(isSelected.Value)
        {
            DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, size.width, size.height, RED);
        }
    }

    DrawText(TextFormat("Gold: %d", scene.gold), -200, -200, 12, WHITE);
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

        auto view = scene.registry.view<EntityPosition, EntitySize, IsSelected>();
        for(auto entity : view)
        {
            IsSelected& isSelected = view.get<IsSelected>(entity);
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

            if(isSelected.Value)
            {
                mouseInfo->selectedUnits++;
            }
        }

        mouseInfo->isSelecting = false;
    }
}

void setSelectedCell(Scene& scene, MouseInfo mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid)
{
    auto view = scene.registry.view<EntityPosition, IsSelected, SelectedCell>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        IsSelected& isSelected = view.get<IsSelected>(entity);
        SelectedCell& cell = view.get<SelectedCell>(entity);
        Pair startGridCell = GetGridPosition(GetPositionOnMap(entityPosition.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);

        if(isSelected.Value && mouseInfo.giveNewTarget)
        {
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, mouseInfo.gridCell, grid);
            cell.pair = targetGridCell;
        }
    }
}

void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid, Camera2D camera)
{
    auto view = scene.registry.view<EntityPosition, IsSelected, Path, IsMoved, TaskState, TaskPositions, TaskStateChanged>();
    for(auto entity : view)
    {
        IsSelected& isSelected = view.get<IsSelected>(entity);
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Path& path = view.get<Path>(entity);
        IsMoved& isMoved = view.get<IsMoved>(entity);
        TaskState& state = view.get<TaskState>(entity);
        TaskStateChanged& stateChanged = view.get<TaskStateChanged>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        Pair startGridCell = GetGridPosition(GetPositionOnMap(entityPosition.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);

        if(isSelected.Value && mouseInfo->giveNewTarget)
        {
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, mouseInfo->gridCell, grid);

            // Prevent quirky behaviour
            // Delete the gridposition where it came from
            Path newPath = GetPath(mapInfo, startGridCell, targetGridCell, grid);
            Pair first = newPath.back();
            if(first.first == startGridCell.first && first.second == startGridCell.second){
                newPath.pop_back();
            }

            path = newPath;
            isMoved = true;
        }

        // Find a better way to do this.
        // StateChanged is used so that in setTargetPosition the path will actually be popped back.
        // This means that the path won't reinitialize again.
        if((state.Value == TaskState::TO_RESOURCE) && stateChanged.Value)
        {
            Pair resourceGridCell = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, resourceGridCell, grid);
            path = GetPath(mapInfo, startGridCell, targetGridCell, grid);
        }
        else if((state.Value == TaskState::TO_BASE) && stateChanged.Value)
        {
            Pair baseGridCell = GetGridPosition(GetPositionOnMap(taskPositions.basePosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, baseGridCell, grid);
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
        float diagonalSpeed = speed / (sqrt(2));    

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