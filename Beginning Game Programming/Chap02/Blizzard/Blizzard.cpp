//-----------------------------------------------------------------
// Blizzard Application
// C++ Source - Blizzard.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Blizzard.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Blizzard"),
                             TEXT("Blizzard"), IDI_BLIZZARD, IDI_BLIZZARD_SM);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(15);
    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Seed the random number generator
    srand(GetTickCount());
}

void GameEnd()
{
    // Cleanup the game engine
    delete g_pGame;
}

void GameActivate(HWND hWindow)
{
    HDC   hDC;
    RECT  rect;
    // Draw activation text on the game screen
    GetClientRect(hWindow, &rect);
    hDC = GetDC(hWindow);
    DrawText(hDC, TEXT("Here comes the blizzard!"), -1, &rect,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ReleaseDC(hWindow, hDC);
}

void GameDeactivate(HWND hWindow)
{
    HDC   hDC;
    RECT  rect;
    // Draw deactivation text on the game screen
    GetClientRect(hWindow, &rect);
    hDC = GetDC(hWindow);
    DrawText(hDC, TEXT("The blizzard has passed."), -1, &rect,
             DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ReleaseDC(hWindow, hDC);
}

void GamePaint(HDC hDC)
{
}

void GameCycle()
{
    HDC   hDC;
    HWND  hWindow = g_pGame->GetWindow();
    // Draw the snowflake icon at random positions on the game screen
    hDC = GetDC(hWindow);
    DrawIcon(hDC, rand() % g_pGame->GetWidth(), rand() % g_pGame->GetHeight(),
             (HICON)(WORD)GetClassLong(hWindow, GCL_HICON));
    ReleaseDC(hWindow, hDC);
}
