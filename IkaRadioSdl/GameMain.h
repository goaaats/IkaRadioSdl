#pragma once
#include <SDL.h>
#include "SongTrack.h"

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

   SongTrack m_songTrack;

   //Assets
   SDL_Surface* m_TitleImg = NULL;

   //Game variables
   bool m_hasQuit = false;
   GameState m_gameState = GameState::Title;

   //Event handler
   SDL_Event m_eventHandler;
};

