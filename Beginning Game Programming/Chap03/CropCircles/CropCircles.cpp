//-----------------------------------------------------------------
// Crop Circles Application
// C++ Source - CropCircles.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "CropCircles.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Crop Circles"),
                             TEXT("Crop Circles"), IDI_CROPCIRCLES, IDI_CROPCIRCLES_SM);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate (yes, it's deliberately slow)
    g_pGame->SetFrameRate(1);
    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Seed the random number generator
    srand(GetTickCount());
    // Set the position and size of the initial crop circle
    g_rcRectangle.left = g_pGame->GetWidth() * 2 / 5;
    g_rcRectangle.top = g_pGame->GetHeight() * 2 / 5;
    g_rcRectangle.right = g_rcRectangle.left + g_pGame->GetWidth() / 10;
    g_rcRectangle.bottom = g_rcRectangle.top + g_pGame->GetWidth() / 10;
}

void GameEnd()
{
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
    // Draw a dark yellow field as the background for the crop circles
    RECT    rect;
    GetClientRect(g_pGame->GetWindow(), &rect);
    HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 0));   // dark yellow color
    FillRect(hDC, &rect, hBrush);
    DeleteObject(hBrush);
}

void GameCycle()
{
    RECT        rect;
    HDC         hDC;
    HWND        hWindow = g_pGame->GetWindow();
    // Remember the location of the last crop circle
    int iXLast = g_rcRectangle.left +
                 (g_rcRectangle.right - g_rcRectangle.left) / 2;
    int iYLast = g_rcRectangle.top +
                 (g_rcRectangle.bottom - g_rcRectangle.top) / 2;
    // Randomly alter the size and position of the new crop circle
    GetClientRect(g_pGame->GetWindow(), &rect);
    int iInflation = (rand() % 17) - 8; // increase or decrease size by up to 8
    InflateRect(&g_rcRectangle, iInflation, iInflation);
    OffsetRect(&g_rcRectangle,
               rand() % (rect.right - rect.left) - g_rcRectangle.left,
               rand() % (rect.bottom - rect.top) - g_rcRectangle.top);
    // Draw a line to the new crop circle
    hDC = GetDC(hWindow);
    HPEN hPen = CreatePen(PS_SOLID, 5, RGB(192, 192, 0)); // light yellow color
    SelectObject(hDC, hPen);
    MoveToEx(hDC, iXLast, iYLast, NULL);
    LineTo(hDC,
           g_rcRectangle.left + (g_rcRectangle.right - g_rcRectangle.left) / 2,
           g_rcRectangle.top + (g_rcRectangle.bottom - g_rcRectangle.top) / 2);
    // Draw the new crop circle
    HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 0));   // lighter yellow color
    SelectObject(hDC, hBrush);
    Ellipse(hDC, g_rcRectangle.left, g_rcRectangle.top,
            g_rcRectangle.right, g_rcRectangle.bottom);
    ReleaseDC(hWindow, hDC);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
