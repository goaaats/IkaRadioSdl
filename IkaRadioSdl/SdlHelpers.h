#pragma once
#include <SDL.h>
#include <string>

class SdlHelpers
{
public:
   static SDL_Surface* LoadSurface( std::string path, SDL_Surface* screenSurface );
   
};

