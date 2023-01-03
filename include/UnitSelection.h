#pragma once

#include "HudElement.h"

#include <string>

class UnitSelection: public HudElement
{
    public:
        int selectedUnits;
        UnitSelection(std::string textureName, Camera2D cam, int width, int height, int marginX, int marginY);
        virtual void Draw() override;
};