#pragma once

#include "Types.h"
#include "Scene.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "HudElement.h"
#include "MiniMap.h"

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080

class Game
{
public:
    Game();
    ~Game();
    void HandleInput();
    void Update(float deltaT);
    void Render(float deltaT);

private:
    MonitorSettings monitorSettings;
    Camera2D camera;
    MapInfo mapInfo;
    std::vector<std::vector<Tile>> grid;
    MouseInfo mouseInfo;
    Boundaries boundaries;
    Scene* scene;
    KeyboardInput* keyboardInput;
    MouseInput* mouseInput;
    std::vector<HudElement*> hud;
    Texture2D background;
    Rectangle selectionRectangle;

    void updateHudElements();
    void drawHudElements();
    void updateMouseInfo(MiniMap* miniMap);
    void updateMapInfo(MiniMap* miniMap);
    void updateMouseSelection(MiniMap* miniMap);
    bool isMouseOverMiniMap(Vector2 worldCurrentPosition, MiniMap* miniMap);
};