/*
    Made by SPMTheCoder - Debjit Sarkar
*/

#include "Game.h"

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include "Windows.h"

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    Game space_invaders;
    while(!space_invaders.ShouldQuit())
        space_invaders.Run();
    return 0;
}

