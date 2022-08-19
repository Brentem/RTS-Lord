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
#include "../include/UnitSelection.h"

#include <stdlib.h>

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080

Scene scene;

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

// You can't initialize the fields here
// because it will cause errors in rendering
// why this is I don't know. -Brent
void CreateUnits()
{
    for(int i = 0; i < 40; i++)
    {
        EntityID entity = scene.NewEntity();
        scene.Assign<EntityPosition>(entity);
        scene.Assign<Texture2D>(entity);
        scene.Assign<EntitySize>(entity);
        scene.Assign<bool>(entity);
    }
}

void InitializeUnits(Texture2D characterTexture)
{
    float posX = 0.0f;
    float posY = 0.0f;

    int counter = 0;

    for(EntityID ent: SceneView<EntityPosition, Texture2D, EntitySize, bool>(scene))
    {
        if(counter % 10 == 0)
        {
            posX = 0.0f;
            posY += 64.0f;
        }

        EntityPosition* position = scene.Get<EntityPosition>(ent);
        Texture2D* texture = scene.Get<Texture2D>(ent);
        EntitySize* size = scene.Get<EntitySize>(ent);
        bool* isSelected = scene.Get<bool>(ent);

        *position = {{posX, posY}, {posX, posY}};
        *texture = characterTexture;
        *size = {32.0f, 32.0f};
        *isSelected = false;

        posX += 64.0f;
        counter++;
    }
}

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
    MouseInfo mouseinfo = {0.0f, 0.0f, 0.0f, 0.0f, 0, false, false, false};
    Texture2D background = Map2DGetBackground(mapInfo, "assets/map1.png", "assets/spritesheet.png");
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png");
    Texture2D characterIcon = LoadTexture("assets/ui/Character_Icon.png");

    Texture2D uiTexture = LoadTexture("assets/ui/UI_placeholder.png");

    std::vector<HudElement*> hud;
    hud.push_back(new MiniMap(background, cam, setting, 150, 150, 2, 10, 10));
    hud.push_back(new HudElement(uiTexture, cam, 1070, 300, 0, 285));
    hud.push_back(new UnitSelection(characterIcon, cam, 20, 20, 230, 530));

    CreateUnits();
    InitializeUnits(characterTexture);

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

        UnitSelection* unitSelection = dynamic_cast<UnitSelection*>(hud[2]);
        unitSelection->selectedUnits = mouseinfo.selectedUnits;
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
    UnloadTexture(characterIcon);

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}