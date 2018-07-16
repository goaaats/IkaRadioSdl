#include "stdafx.h"
#include "GameMain.h"
#include <SDL_mixer.h>
#include "Definitions.h"
#include <SDL_image.h>
#include "SdlHelpers.h"
#include "DrawHelpers.h"


GameMain::GameMain()
{
   if( !Initialize() )
   {
      printf( "Couldn't initialize!" );
      throw exception( "GameMain initialization failed." );
   }

   m_songTrack.LoadNoteData();

   if( !LoadAssets() )
   {
      printf( "Couldn't load assets!" );
      throw exception( "LoadAssets failed." );
   }
}

GameMain::~GameMain()
{
}

bool GameMain::LoadAssets()
{
   bool success = true;

   m_TitleImg = SdlHelpers::LoadSurface( ( ASSET_DIR + "img/Title.png" ).c_str(), m_screenSurface );

   if( m_TitleImg == NULL )
   {
      printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
      success = false;
   }

   m_songTrack.LoadSong( SONG_NAMES[1].first, SONG_NAMES[1].second, "Extreme" );

   return success;
}


bool GameMain::Initialize()
{
   //Initialization flag
   bool success = true;

   //Initialize SDL
   if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
   {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
      success = false;
   }
   else
   {
      //Create window
      m_Window = SDL_CreateWindow( "Ika Radio SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
      if( m_Window == NULL )
      {
         printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
         success = false;
      }
      else
      {
         //Get window surface
         m_screenSurface = SDL_GetWindowSurface( m_Window );
      }

      int imgFlags = IMG_INIT_PNG;
      if( !( IMG_Init( imgFlags ) & imgFlags ) )
      {
         printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
         success = false;
      }

      //Initialize SDL_mixer
      if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
      {
         printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
         success = false;
      }

      //TODO: get window handle?
      if( !BASS_Init( -1, 44100, 0, NULL, NULL ) )
      {
         printf( "BASS could not initialize! BASS Error Code: %d\n", BASS_ErrorGetCode() );
         success = false;
      }
   }

   return success;
}

void GameMain::Finish()
{
   SDL_DestroyWindow( m_Window );
   SDL_Quit();
}

void GameMain::Update()
{
   while( SDL_PollEvent( &m_eventHandler ) != 0 )
   {
      if( m_eventHandler.type == SDL_QUIT )
      {
         m_hasQuit = true;
      }
      else if( m_eventHandler.type == SDL_KEYDOWN )
      {
         HandleKeypress( m_eventHandler.key.keysym.sym );
      }
   }

   switch( m_gameState )
   {
   case Title:
   {
      DrawHelpers::DrawSurfaceStretched( m_TitleImg, m_screenSurface );
   }
   break;
   }

   m_songTrack.Update();


   SDL_UpdateWindowSurface( m_Window );
}

void GameMain::HandleKeypress(SDL_Keycode key)
{
   switch( m_gameState )
   {
   case Title:
   {
      if( key == SDLK_SPACE )
         m_gameState = GameState::InGame;
   }
   break;
   case InGame:
   {
      if( key == SDLK_PLUS )
      {
         if( m_songTrack.IsPlaying() )
            m_songTrack.StopSong();
         else
            m_songTrack.StartSong();
      }
   }
   break;
   }
}

bool GameMain::HasQuit() const
{
   return m_hasQuit;
}

