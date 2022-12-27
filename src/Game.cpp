#include "../include/Game.h"

#include "../include/Monitor.h"
#include "../include/Camera.h"
#include "../include/2DMap.h"
#include "../include/Grid.h"
#include "../include/UnitSelection.h"
#include "../include/Systems.h"
#include "../include/Debug.h"

Game::Game()
{
    monitorSettings = Monitor_GetSettings(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    InitWindow(monitorSettings.monitorWidth, monitorSettings.monitorHeight, "RTS-Lord");
    camera = Camera_Init(monitorSettings.monitorWidth, monitorSettings.monitorHeight,
                                VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    mapInfo = Map2D_Init("assets/map1.png", 32);
    grid = Grid_Init("assets/map1.png", mapInfo);
    mouseInfo = {0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false};
    boundaries = Map2D_GetBoundaries(mapInfo, monitorSettings, camera.zoom);

    background = Map2DGetBackground(mapInfo, "assets/map1.png", "assets/spritesheet.png");
    characterTexture = LoadTexture("assets/Character_Down2.png");
    characterIcon = LoadTexture("assets/ui/Character_Icon.png");
    uiTexture = LoadTexture("assets/ui/UI_placeholder.png");

    scene = new Scene(characterTexture);
    keyboardInput = new KeyboardInput();
    mouseInput = new MouseInput(monitorSettings);

    hud.push_back(new MiniMap(background, camera, monitorSettings, 160, 130, 2, 50, 443));
    hud.push_back(new HudElement(uiTexture, camera, 1070, 300, 0, 300));
    hud.push_back(new UnitSelection(characterIcon, camera, 20, 20, 280, 541));

    SetTargetFPS(60);
}

Game::~Game()
{
    delete scene;
    delete keyboardInput;
    delete mouseInput;

    for(auto& element : hud)
    {
        delete element;
    }

    UnloadTexture(characterTexture);
    UnloadTexture(background);
    UnloadTexture(uiTexture);
    UnloadTexture(characterIcon);
}

void Game::HandleInput()
{
    keyboardInput->HandleInput(&mapInfo);
    mouseInput->HandleInput(&mapInfo);
}

void Game::Update(float deltaT)
{
    Map2D_CheckBoundaries(&mapInfo, boundaries);
    
    Map2D_UpdateMouseInfo(&mouseInfo, &mapInfo);
    updateMouseInfo(static_cast<MiniMap*>(hud[0]));
    updateMapInfo(static_cast<MiniMap*>(hud[0]));
    updateMouseSelection(static_cast<MiniMap*>(hud[0]));
            
    selectionRectangle = Map2D_GetSelectionRectangle(&mouseInfo, camera);

    MovementSystem(*scene, &mouseInfo, mapInfo, selectionRectangle, grid, camera, deltaT);

    updateHudElements();

    UnitSelection* unitSelection = dynamic_cast<UnitSelection*>(hud[2]);
    unitSelection->selectedUnits = mouseInfo.selectedUnits;
}

void Game::Render()
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
		BeginMode2D(camera);
		    // draw the entire background image for the entire world. The camera will clip it to the screen
		    DrawTexture(background, mapInfo.position.x, mapInfo.position.y, WHITE);

            RenderSystem(*scene, mapInfo);

            // Render selection box
            if(mouseInfo.isdragging){
                DrawRectangleLines((int)(selectionRectangle.x) , (int)(selectionRectangle.y), (int)(selectionRectangle.width), (int)(selectionRectangle.height), WHITE);
            }

            //DrawMouseGrid(5, 3, mouseInfo, mapInfo, grid);

            drawHudElements();
            MiniMapCharactersSystem(*scene, dynamic_cast<MiniMap*>(hud[0]));

            // Render some Debug information
            // Debug_DrawDebugInfo(mouseInfo, mapInfo, camera, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, *scene);

		EndMode2D();
    EndDrawing();
}

void Game::updateHudElements()
{
    for(HudElement* element: hud)
    {
        MiniMap* miniMap = dynamic_cast<MiniMap*>(element);

        if(miniMap != nullptr)
        {
            miniMap->Update(mapInfo);
        }
    }
}

void Game::drawHudElements()
{
    for(HudElement* element: hud)
    {
        element->Draw();
    }
}

void Game::updateMouseInfo(MiniMap* miniMap)
{
    if(miniMap == nullptr)
    {
        return;
    }

    int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	Vector2 currentPosition;
	currentPosition.x = (float)mouseX;
	currentPosition.y = (float)mouseY;
	mouseInfo.currentPosition = currentPosition; // Don't think this this mouseInfo variable is ever used.

	Vector2 worldCurrentPosition = GetScreenToWorld2D(currentPosition, camera); 
	mouseInfo.worldCurrentPosition = worldCurrentPosition;

	if(!(miniMap->isActive && IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
		miniMap->isActive = isMouseOverMiniMap(worldCurrentPosition, miniMap);
	}
}

void Game::updateMapInfo(MiniMap* miniMap)
{
    if(miniMap == nullptr)
    {
        return;
    }

    if(!miniMap->isActive)
    {
        return;
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
		// Position on minimap
		int posXOnMinimap = miniMap->position.x + miniMap->miniMapOffSet.x - mouseInfo.worldCurrentPosition.x;
		int posYOnMinimap = miniMap->position.y + miniMap->miniMapOffSet.y- mouseInfo.worldCurrentPosition.y;

        // Don't navigate out the minimap
		if(posXOnMinimap > miniMap->widgetBoundaries.leftBoundary) posXOnMinimap = miniMap->widgetBoundaries.leftBoundary;
		if(posXOnMinimap < miniMap->widgetBoundaries.rightBoundary) posXOnMinimap = miniMap->widgetBoundaries.rightBoundary;
		if(posYOnMinimap > miniMap->widgetBoundaries.upperBoundary) posYOnMinimap = miniMap->widgetBoundaries.upperBoundary;
		if(posYOnMinimap < miniMap->widgetBoundaries.lowerBoundary) posYOnMinimap = miniMap->widgetBoundaries.lowerBoundary;

        float posXOnMap = posXOnMinimap/miniMap->zoomFactor;
		float posYOnMap = posYOnMinimap/miniMap->zoomFactor;

        mapInfo.position.x = posXOnMap;
        mapInfo.position.y = posYOnMap;
        SetOffset(&mapInfo);
    }
}

void Game::updateMouseSelection(MiniMap* miniMap)
{
    if(!miniMap->isActive && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        mouseInfo.selectedUnits = 0;
        mouseInfo.isSelecting = false;

        if(!mouseInfo.isdragging)
        {
            mouseInfo.isdragging = true;
            mouseInfo.startPosition = mouseInfo.currentPosition;
        }
    }
    else
    {
        if(mouseInfo.isdragging)
        {
            mouseInfo.isSelecting = true;
        }

        mouseInfo.isdragging = false;
    }

    mouseInfo.giveNewTarget = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    mouseInfo.worldStartPosition = GetScreenToWorld2D(mouseInfo.startPosition, camera); 
}

bool Game::isMouseOverMiniMap(Vector2 worldCurrentPosition, MiniMap* miniMap)
{
    if(miniMap == nullptr)
    {
        return false;
    }

    bool expression1 = worldCurrentPosition.x > miniMap->position.x;
    bool expression2 = (worldCurrentPosition.x < (miniMap->position.x + miniMap->width)) && expression1;
    bool expression3 = (worldCurrentPosition.y > miniMap->position.y) && expression2;

    return (worldCurrentPosition.y < (miniMap->position.y + miniMap->height)) && expression3;
}
