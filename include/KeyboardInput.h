#pragma once

#include "Command.h"

class KeyboardInput
{
public:
  KeyboardInput();
  ~KeyboardInput();
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