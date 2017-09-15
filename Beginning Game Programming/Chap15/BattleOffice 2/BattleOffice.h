//-----------------------------------------------------------------
// Battle Office 2 Application
// C++ Header - BattleOffice.h
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
Bitmap*     g_pOfficeBitmap;
Bitmap*     g_pTargetBitmap;
Bitmap*     g_pPowBitmap;
Bitmap*     g_pGuyBitmaps[5];
Bitmap*     g_pSmallGuyBitmap;
Bitmap*     g_pGameOverBitmap;
Sprite*     g_pTargetSprite;
Sprite*     g_pPowSprite;
Sprite*     g_pGuySprites[5];
int         g_iGuyDelay[5];
int         g_iGuyMasterDelay;
int         g_iHits;
int         g_iMisses;
BOOL        g_bGameOver;
