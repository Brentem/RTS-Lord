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

#include <stdlib.h>

#define VIEWPORT_WIDTH 800 //800 1920
#define VIEWPORT_HEIGHT 600 //600 1080

void MovementSystem(Scene& scene, Boundaries boundaries)
{
    for(EntityID ent: SceneView<Vector2>(scene))
    {
        Vector2* position = scene.Get<Vector2>(ent);
        
        position->x += 5;

        if(position->x >= boundaries.rightBoundary)
        {
            position->x = 0;
        }
    }
}

void RenderSystem(Scene& scene)
{
    for(EntityID ent: SceneView<Vector2, Texture2D>(scene))
    {
        Vector2* position = scene.Get<Vector2>(ent);
        Texture2D* texture = scene.Get<Texture2D>(ent);

        DrawTexture(*texture, position->x, position->y, WHITE);
    }
}

Scene scene;

EntityID entity1 = scene.NewEntity();
Vector2* position1 = scene.Assign<Vector2>(entity1);
Texture2D* texture1 = scene.Assign<Texture2D>(entity1);

EntityID entity2 = scene.NewEntity();
Vector2* position2 = scene.Assign<Vector2>(entity2);
Texture2D* texture2 = scene.Assign<Texture2D>(entity2);

EntityID entity3 = scene.NewEntity();
Vector2* position3 = scene.Assign<Vector2>(entity3);
Texture2D* texture3 = scene.Assign<Texture2D>(entity3);


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

    MapInfo mapInfo = Map2D_Init(50, 80, 32);
    MouseInfo mouseinfo = {0.0f, 0.0f, 0.0f, 0.0f, false, false};
    Texture2D background = Map2DGetBackground(mapInfo);
    Boundaries boundaries = Map2D_GetBoundaries(mapInfo, setting, cam.zoom);

    Texture2D characterTexture = LoadTexture("assets/Character_Down2.png"); 

    Character character1(characterTexture, {0.0f, 0.0f}, 32.0f, 32.0f);
    Character character2(characterTexture, {60.0f, 30.0f}, 32.0f, 32.0f);

    // ECS Test
    Texture2D ecsTexture = LoadTexture("assets/ECS_Test.png");
    *position1 = {0, 0};
    *texture1 = ecsTexture;

    *position2 = {50, 50};
    *texture2 = ecsTexture;

    *position3 = {100, 100};
    *texture3 = ecsTexture;

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

        // Check if any character should be selected     
        if(mouseinfo.isSelecting){
            Rectangle selectionRectangleOnMap = {selectionRectangle.x - mapInfo.offSet.x, selectionRectangle.y - mapInfo.offSet.y , selectionRectangle.width, selectionRectangle.height};
            character1.updateIsSelected(selectionRectangleOnMap);
            character2.updateIsSelected(selectionRectangleOnMap);
            mouseinfo.isSelecting = false;
        }

        // Give characters a new target position, if needed
        Vector2 currentMousePositionOnMap = (Vector2) {mouseinfo.worldCurrentPosition.x - mapInfo.offSet.x, mouseinfo.worldCurrentPosition.y - mapInfo.offSet.y};
        if(character1.getIsSelected() && mouseinfo.giveNewTarget){
            character1.setTargetPosition(currentMousePositionOnMap);
        }
        if(character2.getIsSelected() && mouseinfo.giveNewTarget){
            character2.setTargetPosition(currentMousePositionOnMap);
        }
        mouseinfo.giveNewTarget = false;

        // ECS Test
        //MovementSystem(scene, boundaries);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

		    BeginMode2D(cam);

		        // draw the entire background image for the entire world. The camera will clip it to the screen
		        DrawTexture(background, mapInfo.position.x, mapInfo.position.y, WHITE);

                // Render character1
                Vector2 characterPosition = character1.getPosition();
                Vector2 characterPositionOnMap = { characterPosition.x + mapInfo.offSet.x, characterPosition.y + mapInfo.offSet.y };
                Rectangle frameRec = { 0.0f, 0.0f, 32.0f, 32.0f };
                DrawTextureRec(characterTexture, frameRec, characterPositionOnMap, WHITE);

                if(character1.getIsSelected()){
                    DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, 32,32, RED);
                }

                // Render character2
                characterPosition = character2.getPosition();
                characterPositionOnMap = { characterPosition.x + mapInfo.offSet.x, characterPosition.y + mapInfo.offSet.y };
                DrawTextureRec(characterTexture, frameRec, characterPositionOnMap, WHITE);

                if(character2.getIsSelected()){
                    DrawRectangleLines(characterPositionOnMap.x, characterPositionOnMap.y, 32,32, RED);
                }

                // Render selection box
                if(mouseinfo.isdragging){
                    DrawRectangleLines((int)(selectionRectangle.x) , (int)(selectionRectangle.y), (int)(selectionRectangle.width), (int)(selectionRectangle.height), WHITE);
                }

                // Render some Debug information
                //Debug_DrawDebugInfo(mouseinfo, mapInfo, cam, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, &character1);

                RenderSystem(scene);

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