extern "C"
{
    #include "raylib.h"
}

#include "../include/2DMap.h"
#include "../include/Debug.h"
#include "../include/Monitor.h"
#include "../include/Camera.h"
#include "../include/Scene.h"
#include "../include/Systems.h" 
#include "../include/Grid.h"
#include "../include/Pathfinding.h"
#include "../include/MiniMap.h"
#include "../include/UnitSelection.h"

#include <stdlib.h>

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080


void UpdateHudElements(std::vector<HudElement*> hud, MapInfo mapInfo)
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

void DrawHudElements(std::vector<HudElement*> hud)
{
    for(HudElement* element: hud)
    {
        element->Draw();
    }
}

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Init for different monitor resolutions => zoom to VIEWPORT_WIDTH/VIEWPORT_HEIGHT
    MonitorSettings setting = Monitor_GetSettings(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    InitWindow(setting.monitorWidth, setting.monitorHeight, "RTS-Lord");
    ToggleFullscreen();

	// setup a camera
	Camera2D cam = Camera_Init(setting.monitorWidth, setting.monitorHeight,
                                VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    MapInfo mapInfo = Map2D_Init("assets/map1.png", 32);
    std::vector<std::vector<Tile>> grid = Grid_Init("assets/map1.png", mapInfo);
    MouseInfo mouseInfo = {0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false};
    Texture2D background = Map2DGetBackground(mapInfo, "assets/map1.png", "assets/spritesheet.png");
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png");
    Texture2D characterIcon = LoadTexture("assets/ui/Character_Icon.png");

    Texture2D uiTexture = LoadTexture("assets/ui/UI_placeholder.png");

    Scene scene(characterTexture);

    std::vector<HudElement*> hud;
    hud.push_back(new MiniMap(background, cam, setting, 160, 130, 2, 50, 443));
    hud.push_back(new HudElement(uiTexture, cam, 1070, 300, 0, 300));
    hud.push_back(new UnitSelection(characterIcon, cam, 20, 20, 280, 541));

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        Map2D_HandleKeyboardInput(&mapInfo);
        Map2D_HandleMouseInput(&mapInfo, &mouseInfo, setting, dynamic_cast<MiniMap*>(hud[0]), cam);
        Map2D_CheckBoundaries(&mapInfo, boundaries);
        Map2D_UpdateMouseInfo(&mouseInfo, &mapInfo);
        
        Rectangle selectionRectangle = Map2D_GetSelectionRectangle(&mouseInfo, cam);

        MovementSystem(scene, &mouseInfo, mapInfo, selectionRectangle, grid, cam);

        UpdateHudElements(hud, mapInfo);

        UnitSelection* unitSelection = dynamic_cast<UnitSelection*>(hud[2]);
        unitSelection->selectedUnits = mouseInfo.selectedUnits;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, mapInfo.position.x, mapInfo.position.y, WHITE);

                RenderSystem(scene, mapInfo);

                // Render selection box
                if(mouseInfo.isdragging){
                    DrawRectangleLines((int)(selectionRectangle.x) , (int)(selectionRectangle.y), (int)(selectionRectangle.width), (int)(selectionRectangle.height), WHITE);
                }

                //DrawMouseGrid(5, 3, mouseInfo, mapInfo, grid);

                DrawHudElements(hud);
                MiniMapCharactersSystem(scene, dynamic_cast<MiniMap*>(hud[0]));

                // Render some Debug information
                // Debug_DrawDebugInfo(mouseInfo, mapInfo, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);


		    EndMode2D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(characterTexture);
    UnloadTexture(background);
    UnloadTexture(uiTexture);
    UnloadTexture(characterIcon);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}