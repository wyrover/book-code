//-----------------------------------------------------------------
// Battle Office 2 Application
// C++ Source - BattleOffice.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "BattleOffice.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Battle Office 2"),
                             TEXT("Battle Office 2"), IDI_BATTLEOFFICE, IDI_BATTLEOFFICE_SM, 500, 400);

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
    g_pOfficeBitmap = new Bitmap(hDC, IDB_OFFICE, g_hInstance);
    g_pTargetBitmap = new Bitmap(hDC, IDB_TARGET, g_hInstance);
    g_pPowBitmap = new Bitmap(hDC, IDB_POW, g_hInstance);
    g_pGuyBitmaps[0] = new Bitmap(hDC, IDB_GUY1, g_hInstance);
    g_pGuyBitmaps[1] = new Bitmap(hDC, IDB_GUY2, g_hInstance);
    g_pGuyBitmaps[2] = new Bitmap(hDC, IDB_GUY3, g_hInstance);
    g_pGuyBitmaps[3] = new Bitmap(hDC, IDB_GUY4, g_hInstance);
    g_pGuyBitmaps[4] = new Bitmap(hDC, IDB_GUY5, g_hInstance);
    g_pSmallGuyBitmap = new Bitmap(hDC, IDB_SMALLGUY, g_hInstance);
    g_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, g_hInstance);
    // Create the target, pow, and guy sprites
    RECT    rcBounds = { 0, 0, 500, 400 };
    g_pTargetSprite = new Sprite(g_pTargetBitmap, rcBounds, BA_STOP);
    g_pTargetSprite->SetZOrder(4);
    g_pGame->AddSprite(g_pTargetSprite);
    g_pPowSprite = new Sprite(g_pPowBitmap, rcBounds, BA_STOP);
    g_pPowSprite->SetZOrder(3);
    g_pPowSprite->SetHidden(TRUE);
    g_pGame->AddSprite(g_pPowSprite);
    g_pGuySprites[0] = new Sprite(g_pGuyBitmaps[0], rcBounds);
    g_pGuySprites[0]->SetPosition(92, 175);
    g_pGuySprites[0]->SetZOrder(2);
    g_pGuySprites[0]->SetHidden(TRUE);
    g_pGame->AddSprite(g_pGuySprites[0]);
    g_pGuySprites[1] = new Sprite(g_pGuyBitmaps[1], rcBounds);
    g_pGuySprites[1]->SetPosition(301, 184);
    g_pGuySprites[1]->SetZOrder(2);
    g_pGuySprites[1]->SetHidden(TRUE);
    g_pGame->AddSprite(g_pGuySprites[1]);
    g_pGuySprites[2] = new Sprite(g_pGuyBitmaps[2], rcBounds);
    g_pGuySprites[2]->SetPosition(394, 61);
    g_pGuySprites[2]->SetZOrder(2);
    g_pGuySprites[2]->SetHidden(TRUE);
    g_pGame->AddSprite(g_pGuySprites[2]);
    rcBounds.left = 340;
    g_pGuySprites[3] = new Sprite(g_pGuyBitmaps[3], rcBounds, BA_WRAP);
    g_pGuySprites[3]->SetNumFrames(4);
    g_pGuySprites[3]->SetPosition(500, 10);
    g_pGuySprites[3]->SetVelocity(-3, 0);
    g_pGuySprites[3]->SetZOrder(1);
    g_pGuySprites[3]->SetHidden(TRUE);
    g_pGame->AddSprite(g_pGuySprites[3]);
    rcBounds.left = 385;
    g_pGuySprites[4] = new Sprite(g_pGuyBitmaps[4], rcBounds, BA_WRAP);
    g_pGuySprites[4]->SetNumFrames(4);
    g_pGuySprites[4]->SetFrameDelay(5);
    g_pGuySprites[4]->SetPosition(260, 60);
    g_pGuySprites[4]->SetVelocity(5, 0);
    g_pGuySprites[4]->SetZOrder(1);
    g_pGuySprites[4]->SetHidden(TRUE);
    g_pGame->AddSprite(g_pGuySprites[4]);
    // Initialize the remaining global variables
    g_iGuyMasterDelay = 50;
    g_iHits = 0;
    g_iMisses = 0;
    g_bGameOver = FALSE;
    // Play the background music
    g_pGame->PlayMIDISong(TEXT("Music.mid"));
}

void GameEnd()
{
    // Close the MIDI player for the background music
    g_pGame->CloseMIDIPlayer();
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    // Cleanup the bitmaps
    delete g_pOfficeBitmap;
    delete g_pTargetBitmap;
    delete g_pPowBitmap;

    for (int i = 0; i < 5; i++)
        delete g_pGuyBitmaps[i];

    delete g_pSmallGuyBitmap;
    delete g_pGameOverBitmap;
    // Cleanup the sprites
    g_pGame->CleanupSprites();
    // Cleanup the game engine
    delete g_pGame;
}

void GameActivate(HWND hWindow)
{
    // Resume the background music
    g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    // Pause the background music
    g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background office
    g_pOfficeBitmap->Draw(hDC, 0, 0);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);
    // Draw the number of guys who were hit
    TCHAR szText[64];
    RECT  rect = { 237, 360, 301, 390 };
    wsprintf(szText, "%d", g_iHits);
    DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    // Draw the number of guys who were missed (got away)
    for (int i = 0; i < g_iMisses; i++)
        g_pSmallGuyBitmap->Draw(hDC, 389 + (g_pSmallGuyBitmap->GetWidth() * i), 359, TRUE);

    // Draw the game over message, if necessary
    if (g_bGameOver)
        g_pGameOverBitmap->Draw(hDC, 120, 110, TRUE);
}

void GameCycle()
{
    if (!g_bGameOver) {
        // Randomly show and hide the guys
        for (int i = 0; i < 5; i++)
            if (g_pGuySprites[i]->IsHidden()) {
                if (rand() % 60 == 0) {
                    // Show the guy
                    g_pGuySprites[i]->SetHidden(FALSE);

                    // Start the countdown delay
                    if (i == 3) {
                        // Start the guy running left
                        g_iGuyDelay[i] = 80;
                        g_pGuySprites[i]->SetPosition(500, 10);
                    } else if (i == 4) {
                        // Start the guy running right
                        g_iGuyDelay[i] = 45;
                        g_pGuySprites[i]->SetPosition(260, 60);
                    } else
                        // Start the stationary guys
                        g_iGuyDelay[i] = 20 + (rand() % g_iGuyMasterDelay);
                }
            } else {
                if (--g_iGuyDelay[i] == 0) {
                    // Play a sound for the guy getting away
                    PlaySound((LPCSTR)IDW_TAUNT, g_hInstance, SND_ASYNC | SND_RESOURCE);
                    // Hide the guy
                    g_pGuySprites[i]->SetHidden(TRUE);

                    // Increment the misses
                    if (++g_iMisses == 5) {
                        // Play a sound for the game ending
                        PlaySound((LPCSTR)IDW_BOO, g_hInstance, SND_ASYNC | SND_RESOURCE);

                        // End the game
                        for (int i = 0; i < 5; i++)
                            g_pGuySprites[i]->SetHidden(TRUE);

                        g_bGameOver = TRUE;
                        // Pause the background music
                        g_pGame->PauseMIDISong();
                    }
                }
            }

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
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
    // Only check the left mouse button
    if (!g_bGameOver && bLeft) {
        // Temporarily hide the target and pow sprites
        g_pTargetSprite->SetHidden(TRUE);
        g_pPowSprite->SetHidden(TRUE);
        // See if a guy sprite was clicked
        Sprite* pSprite;

        if ((pSprite = g_pGame->IsPointInSprite(x, y)) != NULL) {
            // Play a sound for hitting the guy
            PlaySound((LPCSTR)IDW_WHACK, g_hInstance, SND_ASYNC | SND_RESOURCE);
            // Position and show the pow sprite
            g_pPowSprite->SetPosition(x - (g_pPowSprite->GetWidth() / 2),
                                      y - (g_pPowSprite->GetHeight() / 2));
            g_pPowSprite->SetHidden(FALSE);
            // Hide the guy that was clicked
            pSprite->SetHidden(TRUE);

            // Increment the hits and make the game harder, if necessary
            if ((++g_iHits % 5) == 0)
                if (--g_iGuyMasterDelay == 0)
                    g_iGuyMasterDelay = 1;
        }

        // Show the target sprite again
        g_pTargetSprite->SetHidden(FALSE);
    } else if (g_bGameOver && !bLeft) {
        // Start a new game
        g_bGameOver = FALSE;
        g_iHits = 0;
        g_iMisses = 0;
        // Restart the background music
        g_pGame->PlayMIDISong();
    }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
    // Track the mouse with the target sprite
    g_pTargetSprite->SetPosition(x - (g_pTargetSprite->GetWidth() / 2),
                                 y - (g_pTargetSprite->GetHeight() / 2));
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    return FALSE;
}
