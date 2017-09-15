//-----------------------------------------------------------------
// Slideshow Application
// C++ Source - Slideshow.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Slideshow.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Slideshow"),
                             TEXT("Slideshow"), IDI_SLIDESHOW, IDI_SLIDESHOW_SM);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(1);
    // Store the instance handle
    g_hInstance = hInstance;
    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Create and load the slide bitmaps
    HDC hDC = GetDC(hWindow);
    g_pSlides[0] = new Bitmap(hDC, TEXT("Image1.bmp"));
    g_pSlides[1] = new Bitmap(hDC, TEXT("Image2.bmp"));
    g_pSlides[2] = new Bitmap(hDC, TEXT("Image3.bmp"));
    g_pSlides[3] = new Bitmap(hDC, IDB_IMAGE4, g_hInstance);
    g_pSlides[4] = new Bitmap(hDC, IDB_IMAGE5, g_hInstance);
    g_pSlides[5] = new Bitmap(hDC, 640, 480, RGB(0, 0, 0));
    // Set the first slide
    g_iCurSlide = 0;
}

void GameEnd()
{
    // Cleanup the slide bitmaps
    for (int i = 0; i < g_iNUMSLIDES; i++)
        delete g_pSlides[i];

    // Cleanup the game engine
    delete g_pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
    // Draw the current slide bitmap
    g_pSlides[g_iCurSlide]->Draw(hDC, 0, 0);
}

void GameCycle()
{
    static int iDelay = 0;

    // Establish a 3-second delay before moving to the next slide
    if (++iDelay > 3) {
        // Restore the delay counter
        iDelay = 0;

        // Move to the next slide
        if (++g_iCurSlide == g_iNUMSLIDES)
            g_iCurSlide = 0;

        // Force a repaint to draw the next slide
        InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
    }
}
