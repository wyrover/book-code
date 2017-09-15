//-----------------------------------------------------------------
// Planets Application
// C++ Header - Planets.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   g_hInstance;
GameEngine* g_pGame;
Bitmap*     g_pGalaxyBitmap;
Bitmap*     g_pPlanetBitmap[3];
Sprite*     g_pPlanetSprite[3];
BOOL        g_bDragging;
int         g_iDragPlanet;
