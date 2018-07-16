#include "stdafx.h"
#include "Texture.h"
#include <SDL_image.h>
#include <SDL_ttf.h>


Texture::Texture()
{
   //Initialize
   m_texture = NULL;
   m_width = 0;
   m_height = 0;
}

Texture::~Texture()
{
   //Deallocate
   Free();
}

bool Texture::LoadFromFile( SDL_Renderer* renderer, std::string path )
{
   //Get rid of preexisting texture
   Free();

   //The final texture
   SDL_Texture* newTexture = NULL;

   //Load image at specified path
   SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
   if( loadedSurface == NULL )
   {
      printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
   }
   else
   {
      //Color key image
      SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
      //Create texture from surface pixels
      newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
      if( newTexture == NULL )
      {
         printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
      }
      else
      {
         //Get image dimensions
         m_width = loadedSurface->w;
         m_height = loadedSurface->h;
      }

      //Get rid of old loaded surface
      SDL_FreeSurface( loadedSurface );
   }

   //Return success
   m_texture = newTexture;
   return m_texture != NULL;
}

bool Texture::LoadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer )
{
   //Get rid of preexisting texture
   Free();

   //Render text surface
   SDL_Surface* textSurface = TTF_RenderText_Blended( font, textureText.c_str(), textColor );
   if( textSurface == NULL )
   {
      printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
   }
   else
   {
      //Create texture from surface pixels
      m_texture = SDL_CreateTextureFromSurface( renderer, textSurface );
      if( m_texture == NULL )
      {
         printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
      }
      else
      {
         //Get image dimensions
         m_width = textSurface->w;
         m_height = textSurface->h;
      }

      //Get rid of old surface
      SDL_FreeSurface( textSurface );
   }

   //Return success
   return m_texture != NULL;
}

void Texture::Free()
{
   //Free texture if it exists
   if( m_texture != NULL )
   {
      SDL_DestroyTexture( m_texture );
      m_texture = NULL;
      m_width = 0;
      m_height = 0;
   }
}

void Texture::Render( SDL_Renderer* renderer, int x, int y, int w, int h )
{
   SDL_Rect renderQuad = { x, y, w, h };
   SDL_RenderCopy( renderer, m_texture, NULL, &renderQuad );
}

void Texture::Render( SDL_Renderer* renderer, int x, int y, double scale )
{
   SDL_Rect renderQuad = { x, y, m_width * scale, m_height * scale };
   SDL_RenderCopy( renderer, m_texture, NULL, &renderQuad );
}

int Texture::GetWidth()
{
   return m_width;
}

int Texture::GetHeight()
{
   return m_height;
}
