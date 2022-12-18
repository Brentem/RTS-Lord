#include "../include/MouseInput.h"

#include <raylib.h>

MouseInput::MouseInput(MonitorSettings monitorSettings)
{
    this->monitorSettings = monitorSettings;

    moveScreenUpSlow = new MoveScreenUpCommand(4.0f);
    moveScreenUpFast = new MoveScreenUpCommand(12.0f);

    moveScreenLeftSlow = new MoveScreenLeftCommand(4.0f);
    moveScreenLeftFast = new MoveScreenLeftCommand(12.0f);

    moveScreenDownSlow = new MoveScreenDownCommand(4.0f);
    moveScreenDownFast = new MoveScreenDownCommand(12.0f);

    moveScreenRightSlow = new MoveScreenRightCommand(4.0f);
    moveScreenRightFast = new MoveScreenRightCommand(12.0f);
}

MouseInput::~MouseInput()
{
    delete moveScreenUpSlow;
    delete moveScreenUpFast;
    delete moveScreenLeftSlow;
    delete moveScreenLeftFast;
    delete moveScreenDownSlow;
    delete moveScreenDownFast;
    delete moveScreenRightSlow;
    delete moveScreenRightFast;
}

void MouseInput::HandleInput(MapInfo* mapInfo)
{
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    if(mouseX < 20) moveScreenLeftSlow->execute(mapInfo);
    if(mouseX < 5) moveScreenLeftFast->execute(mapInfo);
    if(mouseX > monitorSettings.monitorWidth-20) moveScreenRightSlow->execute(mapInfo);
    if(mouseX > monitorSettings.monitorWidth-5) moveScreenRightFast->execute(mapInfo);

    if(mouseY < 20) moveScreenUpSlow->execute(mapInfo);
    if(mouseY < 5) moveScreenUpFast->execute(mapInfo);
    if(mouseY > monitorSettings.monitorHeight-40) moveScreenDownSlow->execute(mapInfo);
    if(mouseY > monitorSettings.monitorHeight-25) moveScreenDownFast->execute(mapInfo);
}