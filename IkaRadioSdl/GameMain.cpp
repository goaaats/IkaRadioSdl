#include "stdafx.h"
#include "GameMain.h"
#include <SDL_mixer.h>
#include "Definitions.h"
#include <SDL_image.h>
#include <SDL_ttf.h>


GameMain::GameMain()
{
   if( !Initialize() )
   {
      printf( "Couldn't initialize!" );
      throw exception( "GameMain initialization failed." );
   }

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

   if( !m_titleTexture->LoadFromFile( m_renderer, ASSET_DIR + "img/Title.png" ) )
   {
      printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
      success = false;
   }

   //Open the font
   m_mainFont = TTF_OpenFont( ( ASSET_DIR + "Splatfont2.ttf" ).c_str(), 22 );
   if( m_mainFont == NULL )
   {
      printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
      success = false;
   }

   m_songTrack = new SongTrack(m_mainFont, m_renderer);
   m_songTrack->LoadNoteData();

   m_songTrack->LoadSong( SONG_NAMES[m_selectedSong].first, SONG_NAMES[m_selectedSong].second, "Extreme" );

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

      //Create renderer for window
      m_renderer = SDL_CreateRenderer( m_Window, -1, SDL_RENDERER_ACCELERATED );
      if( m_renderer == NULL )
      {
         printf( "Renderer could not initialize! SDL Error: %s\n", SDL_GetError() );
         success = false;
      }
      else
      {
         //Initialize renderer color
         SDL_SetRenderDrawColor( m_renderer, 0x0, 0x0, 0x0, 0x0 );
      }

      /* Not needed right now
      //Initialize SDL_mixer
      if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
      {
         printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
         success = false;
      }
      */

      //Initialize SDL_ttf
      if( TTF_Init() == -1 )
      {
         printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
   IMG_Quit();
   TTF_Quit();
   SDL_Quit();
}

void GameMain::Update()
{
   auto ticks = SDL_GetTicks();

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

   //Clear screen
   SDL_RenderClear( m_renderer );

   switch( m_gameState )
   {
   case Title:
   {
      //Render texture to screen
      m_titleTexture->Render( m_renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
   }
   break;
   case InGame:
   {
      m_songTrack->Update( ticks );
   }
   break;
   }

   m_fpsTexture->LoadFromRenderedText( "FT: " + std::to_string(ticks - m_lastTicks), SDL_Color{ 0xFF, 0xFF, 0xFF }, m_mainFont, m_renderer );
   m_fpsTexture->Render( m_renderer, 0, 0 );

   //Update screen
   SDL_RenderPresent( m_renderer );

   m_lastTicks = ticks;
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
         if( m_songTrack->IsPlaying() )
            m_songTrack->StopSong();
         else
            m_songTrack->StartSong();
      }

      if( key == SDLK_RIGHT )
      {
         if( !m_songTrack->IsPlaying() )
         {
            if( m_selectedSong != SONG_NAMES.size() - 1 )
               m_selectedSong++;

            m_songTrack->LoadSong( SONG_NAMES[m_selectedSong].first, SONG_NAMES[m_selectedSong].second, "Extreme" );
            printf( "Changed selected song: %i - %s\n", m_selectedSong, SONG_NAMES[m_selectedSong].first.c_str() );
         }
      }

      if( key == SDLK_LEFT )
      {
         if( !m_songTrack->IsPlaying() )
         {
            if( m_selectedSong > 0 )
               m_selectedSong--;

            m_songTrack->LoadSong( SONG_NAMES[m_selectedSong].first, SONG_NAMES[m_selectedSong].second, "Extreme" );
            printf( "Changed selected song: %i - %s\n", m_selectedSong, SONG_NAMES[m_selectedSong].first.c_str() );
         }
      }
   }
   break;
   }
}

bool GameMain::HasQuit() const
{
   return m_hasQuit;
}

