extern "C"
{
    #include "raylib.h"
}

#include "../include/Game.h"
#include "../include/AssetManager.h"
#include <chrono>

int main(void) 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    Game game;
    // //--------------------------------------------------------------------------------------

    // Initialize the elapsed time variable
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Get the elapsed time for the current frame
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> deltaT = currentTime - elapsedTime;

        // Update
        //----------------------------------------------------------------------------------
        game.HandleInput();
        game.Update(deltaT.count());
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        game.Render();
        //----------------------------------------------------------------------------------

        // Update the elapsed time variable for the next frame
        elapsedTime = currentTime;
    }

    AssetManager::DeleteInstance();

    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}