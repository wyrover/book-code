//-----------------------------------------------------------------
// Meteor Defense Application
// C++ Header - MeteorDefense.h
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
Bitmap*           g_pGroundBitmap;
Bitmap*           g_pTargetBitmap;
Bitmap*           g_pCityBitmap;
Bitmap*           g_pMeteorBitmap;
Bitmap*           g_pMissileBitmap;
Bitmap*           g_pExplosionBitmap;
Bitmap*           g_pGameOverBitmap;
StarryBackground* g_pBackground;
Sprite*           g_pTargetSprite;
int               g_iNumCities, g_iScore, g_iDifficulty;
BOOL              g_bGameOver;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void AddMeteor();
