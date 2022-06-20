#include "../include/Systems.h"

#include "../include/2DMap.h"

void checkCollision(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection);
void setTargetPosition(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo);
void updatePosition(Scene& scene);

void MovementSystem(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection)
{
    if(mouseInfo == nullptr)
    {
        return;
    }

    checkCollision(scene, mouseInfo, mapInfo, selection);
    setTargetPosition(scene, mouseInfo, mapInfo);
    updatePosition(scene);
}

void RenderSystem(Scene& scene, MapInfo mapInfo)
{
    for(EntityID ent: SceneView<EntityPosition, Texture2D, bool>(scene))
    {
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
        Texture2D* texture = scene.Get<Texture2D>(ent);
        bool* isSelected = scene.Get<bool>(ent);

        Vector2 characterPosition = entityPosition->currentPosition;
        Vector2 characterPositionOnMap = {(characterPosition.x + mapInfo.offSet.x), (characterPosition.y + mapInfo.offSet.y)};
        Rectangle frameRec = { 0.0f, 0.0f, 32.0f, 32.0f }; //TODO: Replace 32 with EntitySize
        DrawTextureRec(*texture, frameRec, characterPositionOnMap, WHITE);

        if(*isSelected)
        {
            DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, 32,32, RED); //TODO: Replace 32 with EntitySize
        }
    }
}

void checkCollision(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo, Rectangle selection)
{
    if(mouseInfo->isSelecting)
    {
        Rectangle selectionRectangleOnMap = {selection.x - mapInfo.offSet.x, selection.y - mapInfo.offSet.y , selection.width, selection.height};

        for(EntityID ent: SceneView<EntityPosition, bool>(scene))
        {
            bool* isSelected = scene.Get<bool>(ent);
            EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);
            
            Rectangle entityBox = {entityPosition->currentPosition.x, entityPosition->currentPosition.y,
                                    32, 32}; //TODO: Replace 32 with EntitySize
            *isSelected = CheckCollisionRecs(selectionRectangleOnMap, entityBox);
        }

        mouseInfo->isSelecting = false;
    }
}

void setTargetPosition(Scene& scene, MouseInfo* mouseInfo, MapInfo mapInfo)
{
    Vector2 currentMousePositionOnMap = (Vector2) {(mouseInfo->worldCurrentPosition.x - mapInfo.offSet.x), 
                                                    (mouseInfo->worldCurrentPosition.y - mapInfo.offSet.y)};

    for(EntityID ent: SceneView<EntityPosition, bool>(scene))
    {
        bool* isSelected = scene.Get<bool>(ent);
        EntityPosition* entityPosition = scene.Get<EntityPosition>(ent);

        if(*isSelected && mouseInfo->giveNewTarget)
        {
            entityPosition->targetPosition = {(currentMousePositionOnMap.x - 16), (currentMousePositionOnMap.y - 16)}; //TODO: Replace 16 with EntitySize
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