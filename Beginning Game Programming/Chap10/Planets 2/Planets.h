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
HDC         g_hOffscreenDC;
HBITMAP     g_hOffscreenBitmap;
Bitmap*     g_pGalaxyBitmap;
Bitmap*     g_pPlanetBitmap[3];
Sprite*     g_pDragSprite;
