#include "../include/InputHandler.h"

#include <raylib.h>

// Should probable be changed to a KeyBoardInputHandler
InputHandler::InputHandler()
{
    MoveScreenUpCommand* up = new MoveScreenUpCommand();
    MoveScreenLeftCommand* left = new MoveScreenLeftCommand();
    MoveScreenDownCommand* down = new MoveScreenDownCommand();
    MoveScreenRightCommand* right = new MoveScreenRightCommand();

    buttonKeyUp = up;
    buttonKeyLeft = left;
    buttonKeyDown = down;
    buttonKeyRight = right;
    
    buttonKeyW = up;
    buttonKeyA = left;
    buttonKeyS = down;
    buttonKeyD = right;
}

InputHandler::~InputHandler()
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

void InputHandler::handleInput(MapInfo* mapInfo)
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