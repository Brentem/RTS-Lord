#pragma once

#include "Types.h"

class Command
{
public:
    Command(float screenMovementFactor);
    virtual ~Command() {}
    virtual void execute(MapInfo* mapInfo) = 0;

protected:
    float screenMovementFactor; // Maybe not ideal place of variable
};

class MoveScreenUpCommand : public Command
{
public:
    MoveScreenUpCommand(float screenMovementFactor);
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenLeftCommand : public Command
{
public:
    MoveScreenLeftCommand(float screenMovementFactor);
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenDownCommand : public Command
{
public:
    MoveScreenDownCommand(float screenMovementFactor);
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenRightCommand : public Command
{
public:
    MoveScreenRightCommand(float screenMovementFactor);
    virtual void execute(MapInfo* mapInfo) override;
};