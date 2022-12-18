#include "../include/Command.h"

#include "../include/2DMap.h"

Command::Command(float screenMovementFactor)
{
    this->screenMovementFactor = screenMovementFactor;
}

MoveScreenUpCommand::MoveScreenUpCommand(float screenMovementFactor)
: Command(screenMovementFactor)
{
}

void MoveScreenUpCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.y += screenMovementFactor;
    SetOffset(mapInfo);
}

MoveScreenLeftCommand::MoveScreenLeftCommand(float screenMovementFactor)
: Command(screenMovementFactor)
{
}

void MoveScreenLeftCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.x += screenMovementFactor;
    SetOffset(mapInfo);
}

MoveScreenDownCommand::MoveScreenDownCommand(float screenMovementFactor)
: Command(screenMovementFactor)
{
}

void MoveScreenDownCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.y -= screenMovementFactor;
    SetOffset(mapInfo);
}

MoveScreenRightCommand::MoveScreenRightCommand(float screenMovementFactor)
: Command(screenMovementFactor)
{
}

void MoveScreenRightCommand::execute(MapInfo* mapInfo)
{
    mapInfo->position.x -= screenMovementFactor;
    SetOffset(mapInfo);
}