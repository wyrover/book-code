//-----------------------------------------------------------------
// Space Out Application
// C++ Header - SpaceOut.h
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
#include "AlienSprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE         g_hInstance;
GameEngine*       g_pGame;
HDC               g_hOffscreenDC;
HBITMAP           g_hOffscreenBitmap;
Bitmap*           g_pDesertBitmap;
Bitmap*           g_pCarBitmap;
Bitmap*           g_pSmCarBitmap;
Bitmap*           g_pMissileBitmap;
Bitmap*           g_pBlobboBitmap;
Bitmap*           g_pBMissileBitmap;
Bitmap*           g_pJellyBitmap;
Bitmap*           g_pJMissileBitmap;
Bitmap*           g_pTimmyBitmap;
Bitmap*           g_pTMissileBitmap;
Bitmap*           g_pSmExplosionBitmap;
Bitmap*           g_pLgExplosionBitmap;
Bitmap*           g_pGameOverBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pCarSprite;
int               g_iFireInputDelay;
int               g_iNumLives, g_iScore, g_iDifficulty;
BOOL              g_bGameOver;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void AddAlien();
