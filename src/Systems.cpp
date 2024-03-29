#include "../include/Systems.h"

#include "../include/Types.h"
#include "../include/Pathfinding.h"
#include "../include/2DMap.h"
#include "../include/Tasks.h"
#include "../include/Subjects.h"
#include "../include/StateMachine.h"
#include "../include/AnimationManager.h"

#include <stdio.h>

using namespace std;

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setSelectedCell(Scene& scene, MouseInfo mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid);
void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid, Camera2D camera);
void setGatheringPath(Scene& scene, MapInfo mapInfo, vector<vector<Tile>>& grid);
Pair GetValidTargetGridCell(Pair startGridCell, Pair selectedTargetGridCell, vector<vector<Tile>>& grid);
void setTargetPosition(Scene& scene, MapInfo mapInfo, Camera2D camera);
void updatePosition(Scene& scene, float deltaT);

Pair getPair(Vector2 position);
float getPositionIndex(int pairIndex);

UnitStateMachineObserver stateObserver;
Subject unitStateMachineSubject;

void InitSystem()
{
    unitStateMachineSubject.AddObserver(static_cast<Observer*>(&stateObserver));
}

static void RunStateMachine(Scene& scene)
{
    auto view = scene.registry.view<UnitState>();
    for(auto entity : view)
    {
        UnitStateMachine(scene.registry, entity);
    }
}

// Maybe this function does too much
void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, vector<vector<Tile>>& grid, Camera2D camera, float deltaT)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    checkIfSelected(scene, mouseInfo, mapInfo, selection);
    setSelectedCell(scene, *mouseInfo, mapInfo, grid); // This function is important for the other functions to function properly.
    CheckResources(scene, mapInfo, *mouseInfo);
    CheckBases(scene, mapInfo, *mouseInfo, grid);
    CheckResourceClick(scene, unitStateMachineSubject, mapInfo);
    CheckResourceReached(scene, unitStateMachineSubject, mapInfo);
    CheckBaseReached(scene, unitStateMachineSubject, mapInfo);
    setPath(scene, mouseInfo, mapInfo, grid, camera);
    setGatheringPath(scene, mapInfo, grid);
    RunStateMachine(scene);
    setTargetPosition(scene, mapInfo, camera);
    updatePosition(scene, deltaT);
}

void RenderSystem(Scene& scene, MapInfo mapInfo, float deltaT)
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

    auto secondView = scene.registry.view<EntityPosition, Animation, EntitySize, IsSelected>();
    for(auto entity : secondView)
    {
        EntityPosition& entityPosition = secondView.get<EntityPosition>(entity);
        //Texture2D& texture = secondView.get<Texture2D>(entity);
        EntitySize& size = secondView.get<EntitySize>(entity);
        IsSelected& isSelected = secondView.get<IsSelected>(entity);

        Vector2 characterPosition = entityPosition.currentPosition;
        Vector2 characterPositionOnMap = {(characterPosition.x + mapInfo.offSet.x), (characterPosition.y + mapInfo.offSet.y)};

        Animation& animation = secondView.get<Animation>(entity);
        ProcessAnimation(animation, size, deltaT, characterPosition);

        if(isSelected.Value)
        {
            DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, size.width, size.height, DARKBROWN);
        }
    }

    DrawText(TextFormat("Gold: %d", scene.gold), -200, -200, 12, WHITE);
}

void ProcessAnimation(Animation& animation, EntitySize& size, float deltaT, Vector2 characterPositionOnMap){
        animation.framesCounter += animation.speed * deltaT;
        if (animation.framesCounter >= 10){
            animation.framesCounter = 0;
            animation.currentFrame++;

            if (animation.currentFrame > animation.frameCount) animation.currentFrame = 0;
        }
        
        Rectangle frameRec = { 0.0f, 0.0f, size.width, size.height };
        frameRec.x = (float)animation.currentFrame*(float)size.width;
        DrawTextureRec(animation.texture, frameRec, characterPositionOnMap, WHITE);
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
    auto view = scene.registry.view<EntityPosition, IsSelected, Path>();
    for(auto entity : view)
    {
        IsSelected& isSelected = view.get<IsSelected>(entity);
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Path& path = view.get<Path>(entity);
        Pair startGridCell = GetGridPosition(GetPositionOnMap(entityPosition.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);

        if(isSelected.Value && mouseInfo->giveNewTarget)
        {
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, mouseInfo->gridCell, grid);

            // No need to give a new target if the currentTarget is the same as the new Target
            bool giveNewTarget = true;
            if(path.size() > 0){
                Pair currentTarget = path[0];
                giveNewTarget = !(currentTarget.first == targetGridCell.first && currentTarget.second == targetGridCell.second);
            }

            if(giveNewTarget){
                // Prevent quirky behaviour
                // Delete the gridposition where it came from
                Path newPath = GetPath(mapInfo, startGridCell, targetGridCell, grid);
                Pair first = newPath.back();
                if(first.first == startGridCell.first && first.second == startGridCell.second){
                    newPath.pop_back();
                }

                path = newPath;

                unitStateMachineSubject.notify(scene.registry, entity, Event::CLICKED_NEW_POSITION);
            }
        }
    }
    mouseInfo->giveNewTarget = false;
}

void setGatheringPath(Scene& scene, MapInfo mapInfo, vector<vector<Tile>>& grid)
{
    auto view = scene.registry.view<EntityPosition, Path, TaskPositions, GatheringFlags>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Path& path = view.get<Path>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        GatheringFlags& flags = view.get<GatheringFlags>(entity);

        Pair startGridCell = GetGridPosition(GetPositionOnMap(entityPosition.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);

        if((flags.GatheringActivated) && (flags.SetGatheringPath))
        {
            flags.SetGatheringPath = false;
            Pair resourceGridCell = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, resourceGridCell, grid);
            path = GetPath(mapInfo, startGridCell, targetGridCell, grid);
        }
        else if((flags.GatheringActivated) && (flags.SetBasePath))
        {
            flags.SetBasePath = false;
            Pair baseGridCell = GetGridPosition(GetPositionOnMap(taskPositions.basePosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            Pair targetGridCell = GetValidTargetGridCell(startGridCell, baseGridCell, grid);
            path = GetPath(mapInfo, startGridCell, targetGridCell, grid);
        }
    }
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
        else
        {
            unitStateMachineSubject.notify(scene.registry, entity, Event::PATH_EMPTY);
        }
    }

    // // Temporary test
    // auto testView = scene.registry.view<Path, UnitState>();
    // for(auto entity : testView)
    // {
    //     UnitState& state = testView.get<UnitState>(entity);
    //     Path& path = testView.get<Path>(entity);

    //     if((path.empty()) && (state.Value == UnitState::WALKING))
    //     {
    //         unitStateMachineSubject.notify(scene.registry, entity, Event::PATH_EMPTY);
    //     }
    // }
    // // End of Temporary test
}

void updatePosition(Scene& scene, float deltaT)
{
    auto view = scene.registry.view<EntityPosition, Animation>();
    for(auto entity : view)
    {
        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Animation& animation = view.get<Animation>(entity);
        Vector2* currentPosition = &entityPosition.currentPosition;
        Vector2* targetPosition = &entityPosition.targetPosition;

        bool isMovingDiagonally = (targetPosition->x != currentPosition->x) && (targetPosition->y != currentPosition->y);
        
        // TODO should be property of the entity
        float speed = 50.0f * deltaT;   
        // TODO should be property of the entity AND calculated correctly          
        float diagonalSpeed = speed / (sqrt(2));  

        Direction horizontalDirection; 
        Direction verticalDirection; 

        if(targetPosition->x > currentPosition->x){
            horizontalDirection.Value = Direction::E;
            if(isMovingDiagonally){
                currentPosition->x += diagonalSpeed;
            } else{
                currentPosition->x += speed;
            }
            if(targetPosition->x < currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->x < currentPosition->x){
            horizontalDirection.Value = Direction::W;
            if(isMovingDiagonally){
                currentPosition->x -= diagonalSpeed;
            } else{
                currentPosition->x -= speed;
            }
            if(targetPosition->x > currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->y > currentPosition->y){
            verticalDirection.Value = Direction::S;
            if(isMovingDiagonally){
                currentPosition->y += diagonalSpeed;
            } else{
                currentPosition->y += speed;
            }
            if(targetPosition->y < currentPosition->y) currentPosition->y = targetPosition->y;
        }

        if(targetPosition->y < currentPosition->y){
            verticalDirection.Value = Direction::N;
            if(isMovingDiagonally){
                currentPosition->y -= diagonalSpeed;
            } else{
                currentPosition->y -= speed;
            }
            if(targetPosition->y > currentPosition->y) currentPosition->y = targetPosition->y;
        }

        Direction newDirection;
        if(isMovingDiagonally){
            if(horizontalDirection.Value == Direction::E){
                if(verticalDirection.Value == Direction::N){
                    newDirection.Value = Direction::NE;
                } else{
                    newDirection.Value = Direction::SE;
                }
            } else{
                if(verticalDirection.Value == Direction::N){
                    newDirection.Value = Direction::NW;
                } else{
                    newDirection.Value = Direction::SW;
                 }
            }
        } else{
            if(horizontalDirection.Value != Direction::None){
                newDirection.Value = horizontalDirection.Value;
            } else{
                newDirection.Value = verticalDirection.Value;
            }
        }

        if(newDirection.Value != Direction::None && newDirection.Value != entityPosition.direction.Value){
            entityPosition.direction = newDirection;
            AnimationManager* animationManager = AnimationManager::GetInstance();
            animationManager->ChangeDirection(animation, newDirection, "peasant_walking");
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