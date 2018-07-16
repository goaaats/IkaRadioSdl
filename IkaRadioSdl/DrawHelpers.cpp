#include "stdafx.h"
#include "DrawHelpers.h"
#include "Definitions.h"
#include <SDL.h>

void DrawHelpers::DrawSurfaceCenter( SDL_Surface* src, SDL_Surface* dst, double scale )
{
   auto xcenter = SCREEN_WIDTH / 2;
   auto ycenter = SCREEN_HEIGHT / 2;

   auto x = xcenter - ( src->w * scale / 2 );
   auto y = ycenter - ( src->h * scale / 2 );

   SDL_Rect dstRect = { x, y, src->w * scale, src->h * scale };

   SDL_BlitScaled( src, NULL, dst, &dstRect );
}

void DrawHelpers::DrawSurfaceStretched( SDL_Surface* src, SDL_Surface* dst)
{
   SDL_Rect stretchRect;

   stretchRect.x = 0;
   stretchRect.y = 0;
   stretchRect.w = SCREEN_WIDTH;
   stretchRect.h = SCREEN_HEIGHT;

   SDL_BlitScaled( src, NULL, dst, &stretchRect );
}

