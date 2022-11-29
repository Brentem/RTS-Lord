#pragma once

#include "Command.h"

// Should probable be changed to a KeyBoardInputHandler
class InputHandler
{
public:
  InputHandler();
  ~InputHandler();
  void handleInput(MapInfo* mapInfo);

private:
  Command* buttonKeyUp;
  Command* buttonKeyLeft;
  Command* buttonKeyDown;
  Command* buttonKeyRight;

  Command* buttonKeyW;
  Command* buttonKeyA;
  Command* buttonKeyS;
  Command* buttonKeyD;
};