//-----------------------------------------------------------------
// Roids 2 Application
// C++ Header - Roids.h
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
#include "Background.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE         g_hInstance;
GameEngine*       g_pGame;
HDC               g_hOffscreenDC;
HBITMAP           g_hOffscreenBitmap;
Bitmap*           g_pAsteroidBitmap;
Bitmap*           g_pSaucerBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pAsteroids[3];
Sprite*           g_pSaucer;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void UpdateSaucer();
