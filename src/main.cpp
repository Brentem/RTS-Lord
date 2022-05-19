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
#include "../include/Monitor.h"
#include "../include/Camera.h"

#include <stdlib.h>

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080

#include <vector>

std::vector<int> list;

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

    MapInfo info = Map2D_Init(50, 80, 32);
    Texture2D background = Map2DGetBackground(info);
    Boundaries boundaries = Map2D_GetBoundaries(info, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png"); 
    Vector2 characterPosition = {0.0f, 0.0f};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        Map2D_HandleKeyboardInput(&info);
        Map2D_HandleMouseInput(&info, setting);
        Map2D_CheckBoundaries(&info, boundaries);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, info.position.x, info.position.y, WHITE);

                Vector2 position = { characterPosition.x + info.mapWidth/2 + info.position.x, characterPosition.y + info.mapHeight/2 + info.position.y };
                Rectangle frameRec = { 0.0f, 0.0f, 32.0f, 32.0f };
                DrawTextureRec(characterTexture, frameRec, position, WHITE);

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