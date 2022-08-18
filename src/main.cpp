extern "C"
{
    #include "raylib.h"
}

#include "../include/2DMap.h"
#include "../include/Debug.h"
#include "../include/Monitor.h"
#include "../include/Camera.h"
#include "../include/SceneView.h"
#include "../include/Types.h"
#include "../include/Systems.h" 
#include "../include/Grid.h"
#include "../include/Pathfinding.h"
#include "../include/MiniMap.h"

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

Scene scene;

EntityID entity1 = scene.NewEntity();
EntityPosition* position1 = scene.Assign<EntityPosition>(entity1);
Texture2D* texture1 = scene.Assign<Texture2D>(entity1);
EntitySize* size1 = scene.Assign<EntitySize>(entity1);
bool* bool1 = scene.Assign<bool>(entity1);

EntityID entity2 = scene.NewEntity();
EntityPosition* position2 = scene.Assign<EntityPosition>(entity2);
Texture2D* texture2 = scene.Assign<Texture2D>(entity2);
EntitySize* size2 = scene.Assign<EntitySize>(entity2);
bool* bool2 = scene.Assign<bool>(entity2);

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Init for different monitor resolutions => zoom to VIEWPORT_WIDTH/VIEWPORT_HEIGHT
    MonitorSettings setting = Monitor_GetSettings(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    InitWindow(setting.monitorWidth, setting.monitorHeight, "RTS-Lord");
    // ToggleFullscreen();

	// setup a camera
	Camera2D cam = Camera_Init(setting.monitorWidth, setting.monitorHeight,
                                VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    MapInfo mapInfo = Map2D_Init("assets/map1.png", 32);
    std::vector<std::vector<Tile>> grid = Grid_Init("assets/map1.png", mapInfo);
    MouseInfo mouseinfo = {0.0f, 0.0f, 0.0f, 0.0f, false, false};
    Texture2D background = Map2DGetBackground(mapInfo, "assets/map1.png", "assets/spritesheet.png");
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png"); 

    Texture2D uiTexture = LoadTexture("assets/ui/UI_placeholder.png");

    std::vector<HudElement*> hud;
    hud.push_back(new MiniMap(background, cam, setting, 150, 150, 2, 10, 10));
    hud.push_back(new HudElement(uiTexture, cam, 1070, 300, 0, 285));

    // ECS Test
    *position1 = {{0, 0}, {0, 0}};
    *texture1 = characterTexture;
    *size1 = {32.0f, 32.0f};

    *position2 = {{60, 30}, {60, 30}};
    *texture2 = characterTexture;
    *size2 = {32.0f, 32.0f};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        Map2D_HandleKeyboardInput(&mapInfo);
        Map2D_HandleMouseInput(&mapInfo, &mouseinfo, setting, dynamic_cast<MiniMap*>(hud[0]), cam);
        Map2D_CheckBoundaries(&mapInfo, boundaries);
        
        Rectangle selectionRectangle = Map2D_GetSelectionRectangle(&mouseinfo, cam);

        MovementSystem(scene, &mouseinfo, mapInfo, selectionRectangle, grid);

        UpdateHudElements(hud, mapInfo);
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
                if(mouseinfo.isdragging){
                    DrawRectangleLines((int)(selectionRectangle.x) , (int)(selectionRectangle.y), (int)(selectionRectangle.width), (int)(selectionRectangle.height), WHITE);
                }

                DrawHudElements(hud);
                MiniMapCharactersSystem(scene, dynamic_cast<MiniMap*>(hud[0]));

                // Render some Debug information
                //Debug_DrawDebugInfo(mouseinfo, mapInfo, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, miniMapInfo);

		    EndMode2D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(characterTexture);
    UnloadTexture(background);
    UnloadTexture(uiTexture);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}