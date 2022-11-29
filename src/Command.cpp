#include "../include/Command.h"

#include "../include/2DMap.h"

void MoveScreenUpCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.y += 5.0f;
    SetOffset(mapInfo);
}

void MoveScreenLeftCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.x += 5.0f;
    SetOffset(mapInfo);
}

void MoveScreenDownCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.y -= 5.0f;
    SetOffset(mapInfo);
}

void MoveScreenRightCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.x -= 5.0f;
    SetOffset(mapInfo);
}