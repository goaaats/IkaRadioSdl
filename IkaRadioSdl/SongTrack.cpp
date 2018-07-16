#include "stdafx.h"
#include "SongTrack.h"
#include "Definitions.h"
#include <SDL.h>

#include <fstream>
#include <chrono>

SongTrack::SongTrack( TTF_Font* font, SDL_Renderer* renderer ): 
   m_font( font ),
   m_renderer( renderer )
{
}

SongTrack::~SongTrack()
{
}

void SongTrack::LoadNoteData()
{
   ifstream infile{ ASSET_DIR + "IkaRadioNoteData.json" };
   string file_contents{ istreambuf_iterator<char>( infile ), istreambuf_iterator<char>() };

   string err = picojson::parse( m_ikaRadioJson, file_contents );

   if( !err.empty() ) {
      printf( ( "SongTrack: Failed to parse IkaRadioNoteData:\n" + err ).c_str() );
      return;
   }
   
   // check if the type of the value is "object"
   if( !m_ikaRadioJson.is<picojson::object>() ) {
      std::cerr << "JSON is not an object" << std::endl;
      exit( 2 );
   }

   // obtain a const reference to the map, and print the contents
   const picojson::value::object& obj = m_ikaRadioJson.get<picojson::object>();
   for( picojson::value::object::const_iterator i = obj.begin();
        i != obj.end();
        ++i ) {
      std::cout << "Song: " << i->first << std::endl;
   }
   
}

void SongTrack::LoadSong(string name, string dataname, string difficulty)
{
   m_currentSongName = name;
   m_currentSong = BASS_StreamCreateFile( false, ( ASSET_DIR + "song/" + name + ".wav" ).c_str(), 0, 0, 0x20000 | 0x100 );
   //m_currentSong = BASS_StreamCreateFile( false, ( ASSET_DIR + "song/" + name + ".wav" ).c_str(), 0, 0, 0x100 | 0x200000 | 0x20000 );

   const picojson::value::object& obj = m_ikaRadioJson.get<picojson::object>();
   auto test = obj.at(dataname + "_" + difficulty);

   auto noteData = obj.at( dataname + "_" + difficulty ).get<picojson::object>();

   m_endNote = noteData.at( "EndNote" ).get<double>();

   auto arr = noteData.at( "NotesUpR" ).get<picojson::array>();

   for( picojson::value::array::const_iterator i = arr.begin();
        i != arr.end();
        ++i ) {
      m_notesUpR.push_back( static_cast<uint64_t>(i->get<double>()) );
   }

   arr = noteData.at( "NotesUpL" ).get<picojson::array>();

   for( picojson::value::array::const_iterator i = arr.begin();
        i != arr.end();
        ++i ) {
      m_notesUpL.push_back( static_cast<uint64_t>( i->get<double>() ) );
   }

   arr = noteData.at( "NotesDownR" ).get<picojson::array>();

   for( picojson::value::array::const_iterator i = arr.begin();
        i != arr.end();
        ++i ) {
      m_notesDownR.push_back( static_cast<uint64_t>( i->get<double>() ) );
   }

   arr = noteData.at( "NotesDownL" ).get<picojson::array>();

   for( picojson::value::array::const_iterator i = arr.begin();
        i != arr.end();
        ++i ) {
      m_notesDownL.push_back( static_cast<uint64_t>( i->get<double>() ) );
   }
}

void SongTrack::StartSong()
{
   if( m_currentSong == NULL )
   {
      printf( "SongTrack: Song is not initialized...\n" );
   }

   BASS_ChannelPlay( m_currentSong, true );
   m_songStartTick = SDL_GetTicks();

   printf( "SongTrack START: at %llu\n", m_songStartTick );
}

void SongTrack::StopSong()
{
   BASS_ChannelStop( m_currentSong );
}

bool SongTrack::IsPlaying()
{
   return BASS_ChannelIsActive( m_currentSong );
}

void SongTrack::Update( uint64_t ticks )
{
   m_debugText->LoadFromRenderedText( m_currentSongName, SDL_Color{ 0xFF, 0xFF, 0xFF }, m_font, m_renderer );
   m_debugText->Render( m_renderer, 0, 30 );

   if( m_currentSong == NULL )
      return;

   if( !BASS_ChannelIsActive( m_currentSong ) )
      return;

   auto channelPosition = BASS_ChannelGetPosition( m_currentSong, 0 );

   if( channelPosition >= BASS_ChannelGetLength( m_currentSong, 0 ) )
   {
      printf( "SongTrack: Song ended" );
      return;
   }

   double songPos = BASS_ChannelBytes2Seconds( m_currentSong, channelPosition );

   if( songPos == 0 )
      return;

   uint64_t songPosMs = songPos * 1000;

   bool upRHit = false;
   bool downRHit = false;
   bool upLHit = false;
   bool downLHit = false;

   /*
   if( HasNote( songPosMs, m_notesUpR ) )
      upRHit = true;

   if( HasNote( songPosMs, m_notesUpL ) )
      upLHit = true;

   if( HasNote( songPosMs, m_notesDownR ) )
      downRHit = true;

   if( HasNote( songPosMs, m_notesDownL ) )
      downLHit = true;
   */

   string hitLog = "";

   if( upLHit && upRHit )
      hitLog = "Hit UP BOTH";

   if( downLHit && downRHit )
      hitLog = "Hit DOWN BOTH";

   if( ( upLHit && downLHit ) || ( upRHit && downRHit ) || ( upLHit && downRHit ) || ( upRHit && downLHit ) )
      hitLog = "Hit ONESIDE BOTH";

   if( downRHit && downLHit && upRHit && upLHit )
      hitLog = "Hit ALL";

   if( upLHit )
      hitLog = "Hit UP L";

   if( upRHit )
      hitLog = "Hit UP R";

   if( upRHit )
      hitLog = "Hit DOWN R";

   if( upLHit )
      hitLog = "Hit DOWN L";

   //printf( "SongTrack: songPos{%f} songPosMs{%llu} %s\n", songPos, songPosMs, hitLog.c_str() );

   Draw( ticks );
}

void SongTrack::Draw( uint64_t ticks )
{

}

bool SongTrack::HasNote(uint64_t tick, vector<uint64_t> vec)
{
   if( std::find( vec.begin(), vec.end(), tick ) != vec.end() ) {
      return true;
   }
   
   return false;
}

/*
bool SongTrack::HasNoteEstimate( uint64_t tick, vector<uint64_t> vec )
{
   auto const it = std::lower_bound( vec.begin(), vec.end(), tick );
   if( it == vec.end() ) { return false; }

   if( abs( ( tick - *it ) ) < 5 )
      return true;

   return false;
}
*/

