#include "raylib.h"
#include "level.h"
#include <time.h>

// IMPORTANT:
// Feel free to not use the Level, and redo everything on your own. This is just some boilerplate code to get you started.

// If you want, I suggest manually copying my code from scratch. That should help with understanding how it works a bit better.

int main(void)
{    
    unsigned int seed = (unsigned int)time(0);
    srand(seed);
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(/*ConfigFlags::FLAG_WINDOW_RESIZABLE | */ConfigFlags::FLAG_VSYNC_HINT | ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Behaviour Arises...");

    // SetTargetFPS(60); // Uncomment this if you want to use fixed framerate (I suggest using vsync)
    // SetExitKey(0);    // Uncomment this to disable ESC as exit key

    Level level;

    level.reset();
    level.initialize();

    while (!WindowShouldClose())
    {
        level.input();

        // Update
        level.update();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        level.draw();

        EndDrawing();
    }

    CloseWindow();
    level.reset();

    return 0;
}