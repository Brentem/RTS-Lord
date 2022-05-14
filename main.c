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

#define VIEWPORT_WIDTH 1920
#define VIEWPORT_HEIGHT 1080

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Init for different monitor resolutions => zoom to VIEWPORT_WIDTH/VIEWPORT_HEIGHT
    InitWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "raylib");

    int currentMonitor = GetCurrentMonitor(); 
    int monitorWidth = GetMonitorWidth(currentMonitor);
    int monitorHeight = GetMonitorHeight(currentMonitor);
    CloseWindow(); 
    InitWindow(monitorWidth, monitorHeight, "raylib");
    ToggleFullscreen();

    float zoomX = (float)monitorWidth/VIEWPORT_WIDTH;
    float zoomY = (float)monitorHeight/VIEWPORT_HEIGHT;
    float zoom = zoomX;
    if(zoomY < zoomX) zoom = zoomY;

	// setup a camera
	Camera2D cam = { 0 };
	cam.zoom = zoom;
	// center the camera on the middle of the screen
	cam.offset = (Vector2){ monitorWidth / 2, monitorHeight / 2 };
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Texture2D background = Map2DGetBackground();
    int mapWidth = Map2dGetWidth();
    int mapHeight = Map2dGetHeight();
    int posX = (mapWidth/2) *-1;
    int posY = (mapHeight/2) *-1;

    int vertikalScrollSpace = mapHeight-VIEWPORT_HEIGHT;
    int upperBoundary = posY + (vertikalScrollSpace/2);
    int lowerBoundary = posY - (vertikalScrollSpace/2);

    int horizontalScrollSpace = mapWidth-VIEWPORT_WIDTH;
    int leftBoundary = posX + (horizontalScrollSpace/2);
    int rightBoundary = posX - (horizontalScrollSpace/2);

    int mouseX;
    int mouseY;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) posX += 5.0f;
        if (IsKeyDown(KEY_LEFT)) posX -= 5.0f;
        if (IsKeyDown(KEY_UP)) posY -= 5.0f;
        if (IsKeyDown(KEY_DOWN)) posY += 5.0f;

        mouseX = GetMouseX();
        if (mouseX < 20) posX += 2.0f;
        if (mouseX < 5) posX += 4.0f;
        if (mouseX > monitorWidth-20) posX -= 2.0f;
        if (mouseX > monitorWidth-5) posX -= 4.0f;

        mouseY = GetMouseY();
        if (mouseY < 20) posY += 2.0f;
        if (mouseY < 5) posY += 2.0f;
        if (mouseY > monitorHeight-40) posY -= 2.0f;
        if (mouseY > monitorHeight-25) posY -= 2.0f;

        if(posY > upperBoundary) posY = upperBoundary;
        if(posY < lowerBoundary) posY = lowerBoundary;
        if(posX > leftBoundary) posX = leftBoundary;
        if(posX < rightBoundary) posX = rightBoundary;
     


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, posX, posY, WHITE);

		    EndMode2D();

            // char snum[10];
            // itoa(posY, snum, 10);
            // DrawText(snum, 190, 200, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}