#pragma once
#include <SDL.h>

class DrawHelpers
{
public:
   static void DrawSurfaceCenter( SDL_Surface* src, SDL_Surface* dst, double scale = 1 );
   static void DrawSurfaceStretched( SDL_Surface* src, SDL_Surface* dst );
};

