#pragma once
#include <string>
#include <bass.h>
#include "picojson.h"

using namespace std;

class SongTrack
{
public:
   SongTrack();
   ~SongTrack();

   void LoadNoteData();

   void LoadSong( string name, string dataname, string difficulty );
   void StartSong();
   void StopSong();
   void Update();
   bool IsPlaying();

private:
   bool HasNote( uint64_t tick, vector<uint64_t> vec );
   bool HasNoteEstimate( uint64_t tick, vector<uint64_t> vec );

   HSTREAM m_currentSong;

   vector<uint64_t> m_notesUpL;
   vector<uint64_t> m_notesUpR;
   vector<uint64_t> m_notesDownR;
   vector<uint64_t> m_notesDownL;
   double m_endNote;
   
   uint64_t m_songStartTick;

   picojson::value m_ikaRadioJson;

};

