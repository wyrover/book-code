//-----------------------------------------------------------------
// Roids Application
// C++ Source - Roids.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Roids.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Roids"),
                             TEXT("Roids"), IDI_ROIDS, IDI_ROIDS_SM, 500, 400);

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
    // Create and load the asteroid bitmap
    HDC hDC = GetDC(hWindow);
    g_pAsteroidBitmap = new Bitmap(hDC, IDB_ASTEROID, g_hInstance);
    // Create the starry background
    g_pBackground = new StarryBackground(500, 400);
    // Create the asteroid sprites
    RECT    rcBounds = { 0, 0, 500, 400 };
    Sprite* pSprite;
    pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
    pSprite->SetNumFrames(14);
    pSprite->SetFrameDelay(1);
    pSprite->SetPosition(250, 200);
    pSprite->SetVelocity(-3, 1);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
    pSprite->SetNumFrames(14);
    pSprite->SetFrameDelay(2);
    pSprite->SetPosition(250, 200);
    pSprite->SetVelocity(3, -2);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
    pSprite->SetNumFrames(14);
    pSprite->SetFrameDelay(3);
    pSprite->SetPosition(250, 200);
    pSprite->SetVelocity(-2, -4);
    g_pGame->AddSprite(pSprite);
}

void GameEnd()
{
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    // Cleanup the asteroid bitmap
    delete g_pAsteroidBitmap;
    // Cleanup the background
    delete g_pBackground;
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
    // Draw the background
    g_pBackground->Draw(hDC);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);
}

void GameCycle()
{
    // Update the background
    g_pBackground->Update();
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
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    return FALSE;
}
