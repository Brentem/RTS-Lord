/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "../include/2DMap.h"
#include "../include/Debug.h"
#include "../include/Monitor.h"
#include "../include/Camera.h"
#include "../include/CharacterTest.h"
#include "../include/SceneView.h"
#include "../include/Types.h"
#include "../include/Systems.h" 

#include <stdlib.h>

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080

Scene scene;

EntityID entity1 = scene.NewEntity();
EntityPosition* position1 = scene.Assign<EntityPosition>(entity1);
Texture2D* texture1 = scene.Assign<Texture2D>(entity1);
bool* bool1 = scene.Assign<bool>(entity1);

EntityID entity2 = scene.NewEntity();
EntityPosition* position2 = scene.Assign<EntityPosition>(entity2);
Texture2D* texture2 = scene.Assign<Texture2D>(entity2);
bool* bool2 = scene.Assign<bool>(entity2);

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Init for different monitor resolutions => zoom to VIEWPORT_WIDTH/VIEWPORT_HEIGHT
    MonitorSettings setting = Monitor_GetSettings(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    InitWindow(setting.monitorWidth, setting.monitorHeight, "RTS-Lord");
    //ToggleFullscreen();

	// setup a camera
	Camera2D cam = Camera_Init(setting.monitorWidth, setting.monitorHeight,
                                VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    MapInfo mapInfo = Map2D_Init("assets/map1.png", 32);
    MouseInfo mouseinfo = {0.0f, 0.0f, 0.0f, 0.0f, false, false};
    Texture2D background = Map2DGetBackground(mapInfo, "assets/map1.png", "assets/spritesheet.png");
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png"); 

    // ECS Test
    *position1 = {{0, 0}, {0, 0}};
    *texture1 = characterTexture;

    *position2 = {{60, 30}, {60, 30}};
    *texture2 = characterTexture;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        Map2D_HandleKeyboardInput(&mapInfo);
        Map2D_HandleMouseInput(&mapInfo, &mouseinfo, setting);
        Map2D_CheckBoundaries(&mapInfo, boundaries);
        
        mouseinfo.worldStartPosition = GetScreenToWorld2D(mouseinfo.startPosition, cam); 
        mouseinfo.worldCurrentPosition = GetScreenToWorld2D(mouseinfo.currentPosition, cam);
        Rectangle selectionRectangle = Map2D_GetSelectionRectangle(&mouseinfo, cam);

        //MovementSystem(scene, &mouseinfo, mapInfo, selectionRectangle);

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

                // Render some Debug information
                //Debug_DrawDebugInfo(mouseinfo, mapInfo, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, &character1);

		    EndMode2D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}