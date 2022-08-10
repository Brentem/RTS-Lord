#pragma once

#include "HudElement.h"

// class MiniMap : public HudElement
// {
//     private:
//         int padding;
//         int miniMapWidgetWidth;
//         int miniMapWidgetHeight;

//         int offsetPosX;
//         int offsetPosY;
//         int posXScreen;
//         int posYScreen;

//     public:
//         Vector2 miniMapOffSet;
//         Boundaries widgetBoundaries;
//         float zoomFactor;
//         int width;
//         int height;
//         bool isActive;

//         MiniMap(Texture2D texture, Camera2D cam, MonitorSettings monitorSettings,
//          int width, int height, int padding, int marginX, int marginY);

//         void Update(MapInfo info);
//         virtual void Draw() override;
//         void DrawCharacters(Scene& scene);
// };