#include "../include/Systems.h"

#include "../include/Types.h"
#include "../include/Pathfinding.h"

#include <stdio.h>

using namespace std;

// Temporary solution!
static vector<Path> EntityPaths;

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid);
void setTargetPosition(Scene& scene, MapInfo mapInfo);
void updatePosition(Scene& scene);

Pair getPair(Vector2 position);
float getPositionIndex(int pairIndex);

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection, vector<vector<Tile>>& grid)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    if(EntityPaths.capacity() != MAX_ENTITIES)
    {
        Path initPath;
        EntityPaths.reserve(MAX_ENTITIES);

        for(int i = 0; i < MAX_ENTITIES; i++)
        {
            EntityPaths.push_back(initPath);
        }
    }

    checkIfSelected(scene, mouseInfo, mapInfo, selection);
    setPath(scene, mouseInfo, mapInfo, grid);
    setTargetPosition(scene, mapInfo);
    updatePosition(scene);
}

void RenderSystem(Scene& scene, MapInfo mapInfo)
{
    for(EntityID ent: SceneView<EntityPosition, Texture2D, EntitySize, bool>(scene))
    {
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
        Texture2D* texture = scene.Get<Texture2D>(ent);
        EntitySize* size = scene.Get<EntitySize>(ent);
        bool* isSelected = scene.Get<bool>(ent);

        Vector2 characterPosition = entityPosition->currentPosition;
        Vector2 characterPositionOnMap = {(characterPosition.x + mapInfo.offSet.x), (characterPosition.y + mapInfo.offSet.y)};
        Rectangle frameRec = { 0.0f, 0.0f, size->width, size->height };
        DrawTextureRec(*texture, frameRec, characterPositionOnMap, WHITE);

        if(*isSelected)
        {
            DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, size->width, size->height, RED);
        }
    }
}

void MiniMapCharactersSystem(Scene& scene, MiniMap* miniMap)
{
    if(miniMap == nullptr)
    {
        return;
    }

    for(EntityID ent: SceneView<EntityPosition>(scene))
    {
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);

        Vector2 characterPosition = entityPosition->currentPosition;
		Vector2 characterPositionOnMinimap;
		characterPositionOnMinimap.x = miniMap->position.x + miniMap->padding + miniMap->width/2 + characterPosition.x*miniMap->zoomFactor;
		characterPositionOnMinimap.y = miniMap->position.y + miniMap->padding + miniMap->height/2 + characterPosition.y*miniMap->zoomFactor;
		DrawCircle((int)characterPositionOnMinimap.x, (int)characterPositionOnMinimap.y, 32*miniMap->zoomFactor, BLUE);
    }

    DrawRectangleLines(miniMap->posXScreen, miniMap->posYScreen, miniMap->miniMapWidgetWidth, miniMap->miniMapWidgetHeight, WHITE);
}

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection)
{
    if(mouseInfo->isSelecting)
    {
        Rectangle selectionRectangleOnMap = {selection.x - mapInfo.offSet.x, selection.y - mapInfo.offSet.y , selection.width, selection.height};

        for(EntityID ent: SceneView<EntityPosition, EntitySize, bool>(scene))
        {
            bool* isSelected = scene.Get<bool>(ent);
            EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
            EntitySize* size = scene.Get<EntitySize>(ent);
            
            Rectangle entityBox = {entityPosition->currentPosition.x, entityPosition->currentPosition.y,
                                    size->width, size->height};

            if(mouseInfo->selectedUnits < MAX_UNITS_SELECTED)
            {
                *isSelected = CheckCollisionRecs(selectionRectangleOnMap, entityBox);
            }
            else
            {
                *isSelected = false;
            }

            if(*isSelected)
            {
                mouseInfo->selectedUnits++;
            }
        }

        mouseInfo->isSelecting = false;
    }
}

void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, vector<vector<Tile>>& grid)
{
    for(EntityID ent: SceneView<EntityPosition, bool>(scene))
    {
        bool* isSelected = scene.Get<bool>(ent);
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);

        if(*isSelected && mouseInfo->giveNewTarget)
        {
            Vector2 currentMousePositionOnMap = (Vector2) {(mouseInfo->worldCurrentPosition.x - mapInfo.offSet.x), 
                                                            (mouseInfo->worldCurrentPosition.y - mapInfo.offSet.y)};
            Vector2 currentPositionOnMap = (Vector2){entityPosition->currentPosition.x, entityPosition->currentPosition.y};

            int entityIndex = GetEntityIndex(ent);
            EntityPaths[entityIndex] = GetPath(mapInfo, getPair(currentPositionOnMap), getPair(currentMousePositionOnMap), grid);
        }
    }
    mouseInfo->giveNewTarget = false;
}

void setTargetPosition(Scene& scene, MapInfo mapInfo)
{
    for(EntityID ent: SceneView<EntityPosition>(scene))
    {
        EntityPosition* position = scene.Get<EntityPosition>(ent);

        int entityIndex = GetEntityIndex(ent);

        if((!(EntityPaths[entityIndex].empty())) && (!(EntityPaths.empty())))
        {
            Pair pair = EntityPaths[entityIndex].back();
            position->targetPosition = Vector2{getPositionIndex(pair.first), getPositionIndex(pair.second)};

            if((position->currentPosition.x == position->targetPosition.x) &&
                (position->currentPosition.y == position->targetPosition.y))
            {
                EntityPaths[entityIndex].pop_back();
            }
        }
    }
}

void updatePosition(Scene& scene)
{
    for(EntityID ent: SceneView<EntityPosition>(scene))
    {
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
        Vector2* currentPosition = &entityPosition->currentPosition;
        Vector2* targetPosition = &entityPosition->targetPosition;

        if(targetPosition->x > currentPosition->x){
            currentPosition->x += 1.0f;
            if(targetPosition->x < currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->x < currentPosition->x){
            currentPosition->x -= 1.0f;
            if(targetPosition->x > currentPosition->x) currentPosition->x = targetPosition->x;
        }

        if(targetPosition->y > currentPosition->y){
            currentPosition->y += 1.0f;
            if(targetPosition->y < currentPosition->y) currentPosition->y = targetPosition->y;
        }

        if(targetPosition->y < currentPosition->y){
            currentPosition->y -= 1.0f;
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