#pragma once
#include <string>
#include <bass.h>
#include "picojson.h"
#include <SDL.h>
#include "Texture.h"

using namespace std;

class SongTrack
{
public:
   SongTrack( TTF_Font* font, SDL_Renderer* renderer );
   ~SongTrack();

   void LoadNoteData();

   void LoadSong( string name, string dataname, string difficulty );
   void StartSong();
   void StopSong();
   void Update( uint64_t ticks );
   bool IsPlaying();

private:
   void Draw( uint64_t ticks );

   bool HasNote( uint64_t tick, vector<uint64_t> vec );


   ///// GRAPHICS STUFF

   SDL_Renderer* m_renderer;

   TTF_Font* m_font;
   Texture* m_debugText = new Texture();

   ///// RHYTHM GAME STUFF

   std::string m_currentSongName;
   HSTREAM m_currentSong;

   vector<uint64_t> m_notesUpL;
   vector<uint64_t> m_notesUpR;
   vector<uint64_t> m_notesDownR;
   vector<uint64_t> m_notesDownL;
   double m_endNote;
   
   uint64_t m_songStartTick;

   picojson::value m_ikaRadioJson;

};

