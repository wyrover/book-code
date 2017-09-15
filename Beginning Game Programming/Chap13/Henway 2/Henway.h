//-----------------------------------------------------------------
// Henway 2 Application
// C++ Header - Henway.h
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
Bitmap*     g_pHighwayBitmap;
Bitmap*     g_pChickenBitmap;
Bitmap*     g_pCarBitmaps[4];
Bitmap*     g_pChickenHeadBitmap;
Sprite*     g_pChickenSprite;
int         g_iInputDelay;
int         g_iNumLives;
int         g_iScore;
BOOL        g_bGameOver;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void MoveChicken(int iXDistance, int iYDistance);
