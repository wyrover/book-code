//-----------------------------------------------------------------
// Henway Application
// C++ Source - Henway.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Henway.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Henway"),
                             TEXT("Henway"), IDI_HENWAY, IDI_HENWAY_SM, 465, 400);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(30);
    // Initialize the joystick
    g_pGame->InitJoystick();
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
    g_pHighwayBitmap = new Bitmap(hDC, IDB_HIGHWAY, g_hInstance);
    g_pChickenBitmap = new Bitmap(hDC, IDB_CHICKEN, g_hInstance);
    g_pCarBitmaps[0] = new Bitmap(hDC, IDB_CAR1, g_hInstance);
    g_pCarBitmaps[1] = new Bitmap(hDC, IDB_CAR2, g_hInstance);
    g_pCarBitmaps[2] = new Bitmap(hDC, IDB_CAR3, g_hInstance);
    g_pCarBitmaps[3] = new Bitmap(hDC, IDB_CAR4, g_hInstance);
    g_pChickenHeadBitmap = new Bitmap(hDC, IDB_CHICKENHEAD, g_hInstance);
    // Create the chicken and car sprites
    Sprite* pSprite;
    RECT    rcBounds = { 0, 0, 465, 400 };
    g_pChickenSprite = new Sprite(g_pChickenBitmap, rcBounds, BA_STOP);
    g_pChickenSprite->SetPosition(4, 175);
    g_pChickenSprite->SetVelocity(0, 0);
    g_pChickenSprite->SetZOrder(1);
    g_pGame->AddSprite(g_pChickenSprite);
    pSprite = new Sprite(g_pCarBitmaps[0], rcBounds, BA_WRAP);
    pSprite->SetPosition(70, 0);
    pSprite->SetVelocity(0, 7);
    pSprite->SetZOrder(2);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pCarBitmaps[1], rcBounds, BA_WRAP);
    pSprite->SetPosition(160, 0);
    pSprite->SetVelocity(0, 3);
    pSprite->SetZOrder(2);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pCarBitmaps[2], rcBounds, BA_WRAP);
    pSprite->SetPosition(239, 400);
    pSprite->SetVelocity(0, -5);
    pSprite->SetZOrder(2);
    g_pGame->AddSprite(pSprite);
    pSprite = new Sprite(g_pCarBitmaps[3], rcBounds, BA_WRAP);
    pSprite->SetPosition(329, 400);
    pSprite->SetVelocity(0, -10);
    pSprite->SetZOrder(2);
    g_pGame->AddSprite(pSprite);
    // Initialize the remaining global variables
    g_iInputDelay = 0;
    g_iNumLives = 3;
    g_iScore = 0;
    g_bGameOver = FALSE;
}

void GameEnd()
{
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    // Cleanup the bitmaps
    delete g_pHighwayBitmap;
    delete g_pChickenBitmap;

    for (int i = 0; i < 4; i++)
        delete g_pCarBitmaps[i];

    delete g_pChickenHeadBitmap;
    // Cleanup the sprites
    g_pGame->CleanupSprites();
    // Cleanup the game engine
    delete g_pGame;
}

void GameActivate(HWND hWindow)
{
    // Capture the joystick
    g_pGame->CaptureJoystick();
}

void GameDeactivate(HWND hWindow)
{
    // Release the joystick
    g_pGame->ReleaseJoystick();
}

void GamePaint(HDC hDC)
{
    // Draw the background highway
    g_pHighwayBitmap->Draw(hDC, 0, 0);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);

    // Draw the number of remaining chicken lives
    for (int i = 0; i < g_iNumLives; i++)
        g_pChickenHeadBitmap->Draw(hDC,
                                   406 + (g_pChickenHeadBitmap->GetWidth() * i), 382, TRUE);
}

void GameCycle()
{
    if (!g_bGameOver) {
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
}

void HandleKeys()
{
    if (!g_bGameOver && (++g_iInputDelay > 2)) {
        // Move the chicken based upon key presses
        if (GetAsyncKeyState(VK_LEFT) < 0)
            MoveChicken(-20, 0);
        else if (GetAsyncKeyState(VK_RIGHT) < 0)
            MoveChicken(20, 0);

        if (GetAsyncKeyState(VK_UP) < 0)
            MoveChicken(0, -20);
        else if (GetAsyncKeyState(VK_DOWN) < 0)
            MoveChicken(0, 20);

        // Reset the input delay
        g_iInputDelay = 0;
    }
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
    // Start a new game, if necessary
    if (g_bGameOver) {
        g_iNumLives = 3;
        g_iScore = 0;
        g_bGameOver = FALSE;
    }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
    if (!g_bGameOver && (++g_iInputDelay > 2)) {
        // Check horizontal movement
        if (jsJoystickState & JOY_LEFT)
            MoveChicken(-20, 0);
        else if (jsJoystickState & JOY_RIGHT)
            MoveChicken(20, 0);

        // Check vertical movement
        if (jsJoystickState & JOY_UP)
            MoveChicken(0, -20);
        else if (jsJoystickState & JOY_DOWN)
            MoveChicken(0, 20);

        // Reset the input delay
        g_iInputDelay = 0;
    }

    // Check the joystick button and start a new game, if necessary
    if (g_bGameOver && (jsJoystickState & JOY_FIRE1)) {
        g_iNumLives = 3;
        g_iScore = 0;
        g_bGameOver = FALSE;
    }
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    // See if the chicken was hit
    if (pSpriteHittee == g_pChickenSprite) {
        // Move the chicken back to the start
        g_pChickenSprite->SetPosition(4, 175);

        // See if the game is over
        if (--g_iNumLives > 0)
            MessageBox(g_pGame->GetWindow(), TEXT("Ouch!"), TEXT("Henway"), MB_OK);
        else {
            // Display game over message
            TCHAR szText[64];
            wsprintf(szText, "Game Over! You scored %d points.", g_iScore);
            MessageBox(g_pGame->GetWindow(), szText, TEXT("Henway"), MB_OK);
            g_bGameOver = TRUE;
        }

        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void MoveChicken(int iXDistance, int iYDistance)
{
    // Move the chicken to its new position
    g_pChickenSprite->OffsetPosition(iXDistance, iYDistance);

    // See if the chicken made it across
    if (g_pChickenSprite->GetPosition().left > 400) {
        // Move the chicken back to the start and add to the score
        g_pChickenSprite->SetPosition(4, 175);
        g_iScore += 150;
        MessageBox(g_pGame->GetWindow(), TEXT("You made it!"), TEXT("Henway"),
                   MB_OK);
    }
}
