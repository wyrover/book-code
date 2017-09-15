//-----------------------------------------------------------------
// Stunt Jumper Application
// C++ Header - StuntJumper.h
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
#include "ScrollingBackground.h"
#include "MotorcycleSprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE             g_hInstance;
GameEngine*           g_pGame;
HDC                   g_hOffscreenDC;
HBITMAP               g_hOffscreenBitmap;
Bitmap*               g_pSplashBitmap;
BackgroundLayer*      g_pBGRoadLayer;
BackgroundLayer*      g_pBGTreesLayer;
BackgroundLayer*      g_pBGMountainsLayer;
BackgroundLayer*      g_pBGSkyLayer;
Bitmap*               g_pJumperBitmap;
Bitmap*               g_pBusBitmap;
Bitmap*               g_pRampBitmap[2];
Bitmap*               g_pGameOverBitmap;
ScrollingBackground*  g_pBackground;
MotorcycleSprite*     g_pJumperSprite;
Sprite*               g_pLaunchRampSprite;
Sprite*               g_pLandingRampSprite;
Sprite*               g_pBusSprite[7];
int                   g_iInputDelay;
BOOL                  g_bGameOver;
BOOL                  g_bSplash;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void NewJump(int iNumBuses);
