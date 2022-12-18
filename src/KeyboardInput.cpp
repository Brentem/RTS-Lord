#include "../include/KeyboardInput.h"

#include <raylib.h>

KeyboardInput::KeyboardInput()
{
    MoveScreenUpCommand* up = new MoveScreenUpCommand(5.0f);
    MoveScreenLeftCommand* left = new MoveScreenLeftCommand(5.0f);
    MoveScreenDownCommand* down = new MoveScreenDownCommand(5.0f);
    MoveScreenRightCommand* right = new MoveScreenRightCommand(5.0f);

    buttonKeyUp = up;
    buttonKeyLeft = left;
    buttonKeyDown = down;
    buttonKeyRight = right;
    
    buttonKeyW = up;
    buttonKeyA = left;
    buttonKeyS = down;
    buttonKeyD = right;
}

KeyboardInput::~KeyboardInput()
{
    delete buttonKeyUp;
    delete buttonKeyLeft;
    delete buttonKeyDown;
    delete buttonKeyRight;

    // W, A, S, D are already deleted because contain same pointers as above.

    buttonKeyUp = nullptr;
    buttonKeyLeft = nullptr;
    buttonKeyDown = nullptr;
    buttonKeyRight = nullptr;
    buttonKeyW = nullptr;
    buttonKeyA = nullptr;
    buttonKeyS = nullptr;
    buttonKeyD = nullptr;
}

void KeyboardInput::HandleInput(MapInfo* mapInfo)
{
    if(IsKeyDown(KEY_UP)) buttonKeyUp->execute(mapInfo);
    if(IsKeyDown(KEY_LEFT)) buttonKeyLeft->execute(mapInfo);
    if(IsKeyDown(KEY_DOWN)) buttonKeyDown->execute(mapInfo);
    if(IsKeyDown(KEY_RIGHT)) buttonKeyRight->execute(mapInfo);
    if(IsKeyDown(KEY_W)) buttonKeyW->execute(mapInfo);
    if(IsKeyDown(KEY_A)) buttonKeyA->execute(mapInfo);
    if(IsKeyDown(KEY_S)) buttonKeyS->execute(mapInfo);
    if(IsKeyDown(KEY_D)) buttonKeyD->execute(mapInfo);
}