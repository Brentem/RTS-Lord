#pragma once

#include "Types.h"

class Command
{
public:
  virtual ~Command() {}
  virtual void execute(MapInfo* mapInfo) = 0;
};

class MoveScreenUpCommand : public Command
{
public:
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenLeftCommand : public Command
{
public:
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenDownCommand : public Command
{
public:
    virtual void execute(MapInfo* mapInfo) override;
};

class MoveScreenRightCommand : public Command
{
public:
    virtual void execute(MapInfo* mapInfo) override;
};