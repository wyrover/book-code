//-----------------------------------------------------------------
// Planets Application
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
    g_pGame = new GameEngine(hInstance, TEXT("Planets"),
                             TEXT("Planets"), IDI_PLANETS, IDI_PLANETS_SM, 600, 400);

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
    // Create and load the bitmaps
    HDC hDC = GetDC(hWindow);
    g_pGalaxyBitmap = new Bitmap(hDC, IDB_GALAXY, g_hInstance);
    g_pPlanetBitmap[0] = new Bitmap(hDC, IDB_PLANET1, g_hInstance);
    g_pPlanetBitmap[1] = new Bitmap(hDC, IDB_PLANET2, g_hInstance);
    g_pPlanetBitmap[2] = new Bitmap(hDC, IDB_PLANET3, g_hInstance);
    // Create the planet sprites
    RECT rcBounds = { 0, 0, 600, 400 };
    g_pPlanetSprite[0] = new Sprite(g_pPlanetBitmap[0], rcBounds);
    g_pPlanetSprite[1] = new Sprite(g_pPlanetBitmap[1], rcBounds, BA_WRAP);
    g_pPlanetSprite[2] = new Sprite(g_pPlanetBitmap[2], rcBounds, BA_BOUNCE);
    g_pPlanetSprite[0]->SetPosition(0, 0);
    g_pPlanetSprite[0]->SetVelocity(1, 1);
    g_pPlanetSprite[1]->SetVelocity(2, -1);
    g_pPlanetSprite[2]->SetVelocity(3, -2);
    // Set the initial drag info
    g_bDragging = FALSE;
    g_iDragPlanet = -1;
}

void GameEnd()
{
    // Cleanup the bitmaps
    delete g_pGalaxyBitmap;

    for (int i = 0; i < 3; i++)
        delete g_pPlanetBitmap[i];

    // Cleanup the sprites
    for (int i = 0; i < 3; i++)
        delete g_pPlanetSprite[i];

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

    // Draw the planet sprites
    for (int i = 0; i < 3; i++)
        g_pPlanetSprite[i]->Draw(hDC);
}

void GameCycle()
{
    // Update the planet sprites
    for (int i = 0; i < 3; i++)
        g_pPlanetSprite[i]->Update();

    // Force a repaint to redraw the planets
    InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
    // See if a planet was clicked with the left mouse button
    if (bLeft && !g_bDragging) {
        for (int i = 0; i < 3; i++)
            if (g_pPlanetSprite[i]->IsPointInside(x, y)) {
                // Capture the mouse
                SetCapture(g_pGame->GetWindow());
                // Set the drag state and the drag planet
                g_bDragging = TRUE;
                g_iDragPlanet = i;
                // Simulate a mouse move to get started
                MouseMove(x, y);
                // Don't check for more planets
                break;
            }
    }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
    // Release the mouse
    ReleaseCapture();
    // Stop dragging
    g_bDragging = FALSE;
}

void MouseMove(int x, int y)
{
    if (g_bDragging) {
        // Move the sprite to the mouse cursor position
        g_pPlanetSprite[g_iDragPlanet]->SetPosition(
            x - (g_pPlanetBitmap[g_iDragPlanet]->GetWidth() / 2),
            y - (g_pPlanetBitmap[g_iDragPlanet]->GetHeight() / 2));
        // Force a repaint to redraw the planets
        InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
    }
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}
