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

    MapInfo mapInfo = Map2D_Init(50, 80, 32);
    MouseInfo mouseinfo = {0.0f, 0.0f, 0.0f, 0.0f, false, false};
    Texture2D background = Map2DGetBackground(mapInfo);
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);



    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png"); 


    Character character1(characterTexture, {0.0f, 0.0f}, 32.0f, 32.0f);
    Character character2(characterTexture, {60.0f, 30.0f}, 32.0f, 32.0f);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Update
        //----------------------------------------------------------------------------------
        Map2D_HandleKeyboardInput(&mapInfo);
        Map2D_HandleMouseInput(&mapInfo, &mouseinfo, setting);
        Map2D_CheckBoundaries(&mapInfo, boundaries);

        character1.updatePosition();
        character2.updatePosition();
        
        mouseinfo.worldStartPosition = GetScreenToWorld2D(mouseinfo.startPosition, cam); 
        mouseinfo.worldCurrentPosition = GetScreenToWorld2D(mouseinfo.currentPosition, cam);
        Rectangle selectionRectangle = Map2D_GetSelectionRectangle(&mouseinfo, cam);
        
        // Juiste positie op de map
        //Rectangle collisionRectangle = {selectionRectangle.x - (mapInfo.position.x - (((float)mapInfo.mapWidth /2) *-1)), selectionRectangle.y - (mapInfo.position.y - (((float)mapInfo.mapHeight/2) *-1)), selectionRectangle.width, selectionRectangle.height};
        


        if(mouseinfo.isSelecting){
            character1.updateIsSelected(selectionRectangle);
            character2.updateIsSelected(selectionRectangle);
            mouseinfo.isSelecting = false;
        }

        if(character1.getIsSelected() && mouseinfo.giveNewTarget){
            character1.setTargetPosition(mouseinfo.worldCurrentPosition);
        }
        if(character2.getIsSelected() && mouseinfo.giveNewTarget){
            character2.setTargetPosition(mouseinfo.worldCurrentPosition);
        }

        mouseinfo.giveNewTarget = false;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, mapInfo.position.x, mapInfo.position.y, WHITE);

                Vector2 characterPosition = character1.getPosition();
                Vector2 position = { characterPosition.x + mapInfo.mapWidth/2 + mapInfo.position.x, characterPosition.y + mapInfo.mapHeight/2 + mapInfo.position.y };
                Rectangle frameRec = { 0.0f, 0.0f, 32.0f, 32.0f };
                DrawTextureRec(characterTexture, frameRec, position, WHITE);

                if(character1.getIsSelected()){
                    DrawRectangleLines(position.x, position.y, 32,32, RED);
                }

                characterPosition = character2.getPosition();
                position = { characterPosition.x + mapInfo.mapWidth/2 + mapInfo.position.x, characterPosition.y + mapInfo.mapHeight/2 + mapInfo.position.y };
                DrawTextureRec(characterTexture, frameRec, position, WHITE);

                if(character2.getIsSelected()){
                    DrawRectangleLines(position.x, position.y, 32,32, RED);
                }

                if(mouseinfo.isdragging){
                    DrawRectangleLines((int)(selectionRectangle.x) , (int)(selectionRectangle.y), (int)(selectionRectangle.width), (int)(selectionRectangle.height), WHITE);
                   //DrawRectangleLines((int)(collisionRectangle.x) , (int)(collisionRectangle.y), (int)(collisionRectangle.width), (int)(collisionRectangle.height), BLACK);
                }

                Debug_DrawDebugInfo(mouseinfo, characterPosition, mouseinfo.worldStartPosition, mouseinfo.worldCurrentPosition, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

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