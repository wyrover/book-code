//-----------------------------------------------------------------
// UFO 2 Application
// C++ Header - UFO.h
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
const int   g_iMAXSPEED = 8;
Bitmap*     g_pBackground;
Bitmap*     g_pSaucer[2];
int         g_iSaucerX, g_iSaucerY;
int         g_iSpeedX, g_iSpeedY;
BOOL        g_bSaucerFlame;