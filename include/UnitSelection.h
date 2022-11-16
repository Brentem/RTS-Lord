#pragma once

#include "HudElement.h"

class UnitSelection: public HudElement
{
    public:
        int selectedUnits;
        UnitSelection(Texture2D texture, Camera2D cam, int width, int height, int marginX, int marginY);
        virtual void Draw() override;
};