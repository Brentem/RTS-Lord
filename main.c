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

#include "2DMap.h"

#include <stdlib.h>

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------





    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib");
    int currentMonitor = GetCurrentMonitor(); 
    int monitorWidth = GetMonitorWidth(currentMonitor);
    int monitorHeight = GetMonitorHeight(currentMonitor);
    CloseWindow(); 

    InitWindow(monitorWidth, monitorHeight, "raylib");

    float zoomX = monitorWidth/screenWidth;
    float zoomY = monitorHeight/screenHeight;
    float zoom = zoomX;
    if(zoomY < zoomX) zoom = zoomY;

    //ToggleFullscreen();

	// setup a camera
	Camera2D cam = { 0 };
	cam.zoom = zoom;
	// center the camera on the middle of the screen
	cam.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Texture2D background = Map2DGetBackground();
    int posX = Map2dGetWidth();
    int posY = Map2dGetHeight();
    int mouseX;
    int mouseY;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) posX += 2.0f;
        if (IsKeyDown(KEY_LEFT)) posX -= 2.0f;
        if (IsKeyDown(KEY_UP)) posY -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) posY += 2.0f;

        mouseX = GetMouseX();
        if (mouseX < 20) posX += 2.0f;
        if (mouseX < 5) posX += 2.0f;

        if (mouseX > screenWidth-20) posX -= 2.0f;
        if (mouseX > screenWidth-5) posX -= 2.0f;

        mouseY = GetMouseY();
        if (mouseY < 20) posY += 2.0f;
        if (mouseY < 5) posY += 2.0f;

        if (mouseY > screenHeight-20) posY -= 2.0f;
        if (mouseY > screenHeight-5) posY -= 2.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, posX, posY, WHITE);

		    EndMode2D();

        char snum[5];
        itoa(mouseX, snum, 10);
        DrawText(snum, 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}