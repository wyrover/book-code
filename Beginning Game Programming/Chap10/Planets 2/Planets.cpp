//-----------------------------------------------------------------
// Planets 2 Application
// C++ Source - Planets.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Planets.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Planets 2"),
                             TEXT("Planets 2"), IDI_PLANETS, IDI_PLANETS_SM, 600, 400);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(30);
    // Store the instance handle
    g_hInstance = hInstance;
    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Seed the random number generator
    srand(GetTickCount());
    // Create the offscreen device context and bitmap
    g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
                         g_pGame->GetWidth(), g_pGame->GetHeight());
    SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);
    // Create and load the bitmaps
    HDC hDC = GetDC(hWindow);
    g_pGalaxyBitmap = new Bitmap(hDC, IDB_GALAXY, g_hInstance);
    g_pPlanetBitmap[0] = new Bitmap(hDC, IDB_PLANET1, g_hInstance);
    g_pPlanetBitmap[1] = new Bitmap(hDC, IDB_PLANET2, g_hInstance);
    g_pPlanetBitmap[2] = new Bitmap(hDC, IDB_PLANET3, g_hInstance);
    // Create the planet sprites
    RECT    rcBounds = { 0, 0, 600, 400 };
    Sprite* pSprite;
    pSprite = new Sprite(g_pPlanetBitmap[0], rcBounds, BA_WRAP);
    pSprite->SetVelocity(3, 2);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pPlanetBitmap[1], rcBounds, BA_WRAP);
    pSprite->SetVelocity(4, 1);
    g_pGame->AddSprite(pSprite);
    rcBounds.right = 200;
    rcBounds.bottom = 160;
    pSprite = new Sprite(g_pPlanetBitmap[2], rcBounds, BA_BOUNCE);
    pSprite->SetVelocity(-4, 2);
    g_pGame->AddSprite(pSprite);
    rcBounds.left = 400;
    rcBounds.top = 240;
    rcBounds.right = 600;
    rcBounds.bottom = 400;
    pSprite = new Sprite(g_pPlanetBitmap[2], rcBounds, BA_BOUNCE);
    pSprite->SetVelocity(7, -3);
    g_pGame->AddSprite(pSprite);
    // Set the initial drag info
    g_pDragSprite = NULL;
}

void GameEnd()
{
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    // Cleanup the bitmaps
    delete g_pGalaxyBitmap;

    for (int i = 0; i < 3; i++)
        delete g_pPlanetBitmap[i];

    // Cleanup the sprites
    g_pGame->CleanupSprites();
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
    // Draw the background galaxy
    g_pGalaxyBitmap->Draw(hDC, 0, 0);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);
}

void GameCycle()
{
    // Update the sprites
    g_pGame->UpdateSprites();
    // Obtain a device context for repainting the game
    HWND  hWindow = g_pGame->GetWindow();
    HDC   hDC = GetDC(hWindow);
    // Paint the game to the offscreen device context
    GamePaint(g_hOffscreenDC);
    // Blit the offscreen bitmap to the game screen
    BitBlt(hDC, 0, 0, g_pGame->GetWidth(), g_pGame->GetHeight(),
           g_hOffscreenDC, 0, 0, SRCCOPY);
    // Cleanup
    ReleaseDC(hWindow, hDC);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
    // See if a planet was clicked with the left mouse button
    if (bLeft && (g_pDragSprite == NULL)) {
        if ((g_pDragSprite = g_pGame->IsPointInSprite(x, y)) != NULL) {
            // Capture the mouse
            SetCapture(g_pGame->GetWindow());
            // Simulate a mouse move to get started
            MouseMove(x, y);
        }
    }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
    // Release the mouse
    ReleaseCapture();
    // Stop dragging
    g_pDragSprite = NULL;
}

void MouseMove(int x, int y)
{
    if (g_pDragSprite != NULL) {
        // Move the sprite to the mouse cursor position
        g_pDragSprite->SetPosition(x - (g_pDragSprite->GetWidth() / 2),
                                   y - (g_pDragSprite->GetHeight() / 2));
        // Force a repaint to redraw the sprites
        InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
    }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    // Swap the sprite velocities so that they appear to bounce
    POINT ptSwapVelocity = pSpriteHitter->GetVelocity();
    pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
    pSpriteHittee->SetVelocity(ptSwapVelocity);
    return TRUE;
}
