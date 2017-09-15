//-----------------------------------------------------------------
// Brainiac Application
// C++ Header - Brainiac.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   g_hInstance;
GameEngine* g_pGame;
Bitmap*     g_pTiles[9];
BOOL        g_bTileStates[4][4];
int         g_iTiles[4][4];
POINT       g_ptTile1, g_ptTile2;
int         g_iMatches, g_iTries;