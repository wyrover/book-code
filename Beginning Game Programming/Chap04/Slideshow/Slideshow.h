//-----------------------------------------------------------------
// Slideshow Application
// C++ Header - Slideshow.h
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
const int   g_iNUMSLIDES = 6;
Bitmap*     g_pSlides[g_iNUMSLIDES];
int         g_iCurSlide;
