#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Texture
{
public:
   Texture();
   ~Texture();

   //Loads image
   bool LoadFromFile( SDL_Renderer* renderer, std::string path );
   bool LoadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer );

   //Deallocates texture
   void Free();

   //Renders texture at given point
   void Render( SDL_Renderer* renderer, int x, int y, int w, int h );
   void Render( SDL_Renderer* renderer, int x, int y, double scale = 1 );

   //Gets image dimensions
   int GetWidth();
   int GetHeight();

private:
   //The actual hardware texture
   SDL_Texture * m_texture;

   //Image dimensions
   int m_width;
   int m_height;
};

