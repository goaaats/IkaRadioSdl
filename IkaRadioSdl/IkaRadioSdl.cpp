// IkaRadioSdl.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "GameMain.h"
#ifdef _WIN32
#include <Windows.h>
#endif

int main( int argc, char *argv[] )
{
#ifdef _WIN32
   AllocConsole();
#endif

   GameMain* game = new GameMain();

   while( !game->HasQuit() )
   {
      game->Update();
   }

   game->Finish();

   return 0;
}
