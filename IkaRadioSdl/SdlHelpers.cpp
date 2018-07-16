#include "stdafx.h"
#include "SdlHelpers.h"
#include <SDL_image.h>

SDL_Surface* SdlHelpers::LoadSurface(std::string path, SDL_Surface* screenSurface)
{
   //The final optimized image
   SDL_Surface* optimizedSurface = NULL;

   //Load image at specified path
   SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
   if( loadedSurface == NULL )
   {
      printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
   }
   else
   {
      //Convert surface to screen format
      optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, NULL );
      if( optimizedSurface == NULL )
      {
         printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
      }

      //Get rid of old loaded surface
      SDL_FreeSurface( loadedSurface );
   }

   return optimizedSurface;
}

