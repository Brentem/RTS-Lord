#pragma once

#include "HudElement.h"

#include "Types.h"

class MiniMap : public HudElement
{
    private:
        int offsetPosX;
        int offsetPosY;


    public:
        Vector2 miniMapOffSet;
        Boundaries widgetBoundaries;
        float zoomFactor;
        int width;
        int height;
        int padding;
        int posXScreen;
        int posYScreen;
        int miniMapWidgetWidth;
        int miniMapWidgetHeight;
        bool isActive;

        MiniMap(Texture2D* texture, Camera2D cam, MonitorSettings monitorSettings,
         int width, int height, int padding, int marginX, int marginY);

        void Update(MapInfo info);
        virtual void Draw() override;
};