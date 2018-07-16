#pragma once
#include <SDL.h>
#include "SongTrack.h"
#include <SDL_ttf.h>
#include "Texture.h"

class GameMain
{
   enum GameState
   {
      Title,
      InGame
   };

public:
   GameMain();
   ~GameMain();

   bool Initialize();
   void Finish();

   bool LoadAssets();

   void Update();

   bool HasQuit() const;

private:
   void HandleKeypress( SDL_Keycode key );



   //The window we'll be rendering to
   SDL_Window * m_Window = NULL;
   //The surface contained by the window
   SDL_Surface* m_screenSurface = NULL;
   //The window renderer
   SDL_Renderer* m_renderer = NULL;

   // Last ticks
   uint64_t m_lastTicks;

   //Assets
   Texture* m_titleTexture = new Texture();

   Texture* m_fpsTexture = new Texture();
   TTF_Font* m_mainFont = NULL;

   //Game variables
   bool m_hasQuit = false;
   GameState m_gameState = GameState::Title;
   SongTrack* m_songTrack;
   int m_selectedSong = 2;

   //Event handler
   SDL_Event m_eventHandler;
};

