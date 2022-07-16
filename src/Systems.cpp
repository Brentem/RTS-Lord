#include "../include/Systems.h"

#include "../include/Types.h"
#include "../include/Pathfinding.h"

void checkIfSelected(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo);
void setTargetPosition(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo);
void updatePosition(Scene& scene);

Pair getPair(Vector2 position);

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    checkIfSelected(scene, mouseInfo, mapInfo, selection);
    setPath(scene, mouseInfo, mapInfo);
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
            *isSelected = CheckCollisionRecs(selectionRectangleOnMap, entityBox);
        }

        mouseInfo->isSelecting = false;
    }
}

void setPath(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo)
{
    for(EntityID ent: SceneView<EntityPosition, Path, bool>(scene))
    {
        bool* isSelected = scene.Get<bool>(ent);
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
        Path* entityPath = scene.Get<Path>(ent);

        if(*isSelected && mouseInfo->giveNewTarget)
        {
            Vector2 currentMousePositionOnMap = (Vector2) {(mouseInfo->worldCurrentPosition.x - mapInfo.offSet.x), 
                                                            (mouseInfo->worldCurrentPosition.y - mapInfo.offSet.y)};
            Vector2 currentPositionOnMap = (Vector2){(entityPosition->currentPosition.x - mapInfo.offSet.x),
                                                        (entityPosition->currentPosition.y - mapInfo.offSet.y)};

            *entityPath = GetPath(mapInfo, getPair(currentPositionOnMap), getPair(currentMousePositionOnMap));
            int test = 0;
        }
    }
    mouseInfo->giveNewTarget = false;
}

void setTargetPosition(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo)
{
    Vector2 currentMousePositionOnMap = (Vector2) {(mouseInfo->worldCurrentPosition.x - mapInfo.offSet.x), 
                                                    (mouseInfo->worldCurrentPosition.y - mapInfo.offSet.y)};

    for(EntityID ent: SceneView<EntityPosition, EntitySize, bool>(scene))
    {
        bool* isSelected = scene.Get<bool>(ent);
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
        EntitySize* size = scene.Get<EntitySize>(ent);

        if(*isSelected && mouseInfo->giveNewTarget)
        {
            entityPosition->targetPosition = {(currentMousePositionOnMap.x - size->width/2), (currentMousePositionOnMap.y - size->height/2)};
        }
    }
    mouseInfo->giveNewTarget = false;
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
    int x = (int)((position.x / 16) / 2 + 0.5);
    int y = (int)((position.y / 16) / 2 + 0.5);
    return Pair(x, y);
}