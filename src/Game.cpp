#include "../include/Game.h"

#include "../include/Monitor.h"
#include "../include/Camera.h"
#include "../include/2DMap.h"
#include "../include/Grid.h"
#include "../include/UnitSelection.h"
#include "../include/Systems.h"

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

    hud.push_back(new MiniMap(background, camera, monitorSettings, 160, 130, 2, 50, 443));
    hud.push_back(new HudElement(uiTexture, camera, 1070, 300, 0, 300));
    hud.push_back(new UnitSelection(characterIcon, camera, 20, 20, 280, 541));

    SetTargetFPS(60);
}

Game::~Game()
{
    delete scene;
    delete keyboardInput;

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
    Map2D_HandleMouseInput(&mapInfo, &mouseInfo, monitorSettings, dynamic_cast<MiniMap*>(hud[0]), camera);
}

void Game::Update()
{
    Map2D_CheckBoundaries(&mapInfo, boundaries);
    Map2D_UpdateMouseInfo(&mouseInfo, &mapInfo);
            
    selectionRectangle = Map2D_GetSelectionRectangle(&mouseInfo, camera);

    MovementSystem(*scene, &mouseInfo, mapInfo, selectionRectangle, grid, camera);

    UpdateHudElements();

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

            DrawHudElements();
            MiniMapCharactersSystem(*scene, dynamic_cast<MiniMap*>(hud[0]));

            // Render some Debug information
            // Debug_DrawDebugInfo(mouseInfo, mapInfo, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

		EndMode2D();
    EndDrawing();
}

void Game::UpdateHudElements()
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

void Game::DrawHudElements()
{
    for(HudElement* element: hud)
    {
        element->Draw();
    }
}
