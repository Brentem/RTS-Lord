#pragma once

#include "Command.h"

class MouseInput
{
public:
    MouseInput(MonitorSettings monitorSettings);
    ~MouseInput();
    void HandleInput(MapInfo* mapInfo);

private:
    MonitorSettings monitorSettings;

    MoveScreenUpCommand* moveScreenUpSlow;
    MoveScreenUpCommand* moveScreenUpFast;

    MoveScreenLeftCommand* moveScreenLeftSlow;
    MoveScreenLeftCommand* moveScreenLeftFast;

    MoveScreenDownCommand* moveScreenDownSlow;
    MoveScreenDownCommand* moveScreenDownFast;


    MoveScreenRightCommand* moveScreenRightSlow;
    MoveScreenRightCommand* moveScreenRightFast;
};