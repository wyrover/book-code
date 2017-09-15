//-----------------------------------------------------------------
// Space Out 4 Application
// C++ Source - SpaceOut.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SpaceOut.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Space Out 4"),
                             TEXT("Space Out 4"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450);

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
    // Read the hi scores
    ReadHiScores();
    // Seed the random number generator
    srand(GetTickCount());
    // Create the offscreen device context and bitmap
    g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
                         g_pGame->GetWidth(), g_pGame->GetHeight());
    SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);
    // Create and load the bitmaps
    HDC hDC = GetDC(hWindow);
    g_pSplashBitmap = new Bitmap(hDC, IDB_SPLASH, g_hInstance);
    g_pDesertBitmap = new Bitmap(hDC, IDB_DESERT, g_hInstance);
    g_pCarBitmap = new Bitmap(hDC, IDB_CAR, g_hInstance);
    g_pSmCarBitmap = new Bitmap(hDC, IDB_SMCAR, g_hInstance);
    g_pMissileBitmap = new Bitmap(hDC, IDB_MISSILE, g_hInstance);
    g_pBlobboBitmap = new Bitmap(hDC, IDB_BLOBBO, g_hInstance);
    g_pBMissileBitmap = new Bitmap(hDC, IDB_BMISSILE, g_hInstance);
    g_pJellyBitmap = new Bitmap(hDC, IDB_JELLY, g_hInstance);
    g_pJMissileBitmap = new Bitmap(hDC, IDB_JMISSILE, g_hInstance);
    g_pTimmyBitmap = new Bitmap(hDC, IDB_TIMMY, g_hInstance);
    g_pTMissileBitmap = new Bitmap(hDC, IDB_TMISSILE, g_hInstance);
    g_pSmExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, g_hInstance);
    g_pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, g_hInstance);
    g_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, g_hInstance);
    // Create the starry background
    g_pBackground = new StarryBackground(600, 450);
    // Start the game for demo mode
    g_bDemo = TRUE;
    NewGame();
}

void GameEnd()
{
    // Close the MIDI player for the background music
    g_pGame->CloseMIDIPlayer();
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    // Cleanup the bitmaps
    delete g_pSplashBitmap;
    delete g_pDesertBitmap;
    delete g_pCarBitmap;
    delete g_pSmCarBitmap;
    delete g_pMissileBitmap;
    delete g_pBlobboBitmap;
    delete g_pBMissileBitmap;
    delete g_pJellyBitmap;
    delete g_pJMissileBitmap;
    delete g_pTimmyBitmap;
    delete g_pTMissileBitmap;
    delete g_pSmExplosionBitmap;
    delete g_pLgExplosionBitmap;
    delete g_pGameOverBitmap;
    // Cleanup the background
    delete g_pBackground;
    // Cleanup the sprites
    g_pGame->CleanupSprites();
    // Cleanup the game engine
    delete g_pGame;
    // Save the hi scores
    WriteHiScores();
}

void GameActivate(HWND hWindow)
{
    if (!g_bDemo)
        // Resume the background music
        g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    if (!g_bDemo)
        // Pause the background music
        g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background
    g_pBackground->Draw(hDC);
    // Draw the desert bitmap
    g_pDesertBitmap->Draw(hDC, 0, 371);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);

    if (g_bDemo) {
        // Draw the splash screen image
        g_pSplashBitmap->Draw(hDC, 142, 20, TRUE);
        // Draw the hi scores
        TCHAR szText[64];
        RECT  rect = { 275, 250, 325, 270};
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(255, 255, 255));

        for (int i = 0; i < 5; i++) {
            wsprintf(szText, "%d", g_iHiScores[i]);
            DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            rect.top += 20;
            rect.bottom += 20;
        }
    } else {
        // Draw the score
        TCHAR szText[64];
        RECT  rect = { 460, 0, 510, 30 };
        wsprintf(szText, "%d", g_iScore);
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(255, 255, 255));
        DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

        // Draw the number of remaining lives (cars)
        for (int i = 0; i < g_iNumLives; i++)
            g_pSmCarBitmap->Draw(hDC, 520 + (g_pSmCarBitmap->GetWidth() * i),
                                 10, TRUE);

        // Draw the game over message, if necessary
        if (g_bGameOver)
            g_pGameOverBitmap->Draw(hDC, 170, 100, TRUE);
    }
}

void GameCycle()
{
    if (!g_bGameOver) {
        if (!g_bDemo) {
            // Randomly add aliens
            if ((rand() % g_iDifficulty) == 0)
                AddAlien();
        }

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
    } else if (--g_iGameOverDelay == 0) {
        // Stop the music and switch to demo mode
        g_pGame->PauseMIDISong();
        g_bDemo = TRUE;
        NewGame();
    }
}

void HandleKeys()
{
    if (!g_bGameOver && !g_bDemo) {
        // Move the car based upon left/right key presses
        POINT ptVelocity = g_pCarSprite->GetVelocity();

        if (GetAsyncKeyState(VK_LEFT) < 0) {
            // Move left
            ptVelocity.x = max(ptVelocity.x - 1, -4);
            g_pCarSprite->SetVelocity(ptVelocity);
        } else if (GetAsyncKeyState(VK_RIGHT) < 0) {
            // Move right
            ptVelocity.x = min(ptVelocity.x + 2, 6);
            g_pCarSprite->SetVelocity(ptVelocity);
        }

        // Fire missiles based upon spacebar presses
        if ((++g_iFireInputDelay > 6) && GetAsyncKeyState(VK_SPACE) < 0) {
            // Create a new missile sprite
            RECT  rcBounds = { 0, 0, 600, 450 };
            RECT  rcPos = g_pCarSprite->GetPosition();
            Sprite* pSprite = new Sprite(g_pMissileBitmap, rcBounds, BA_DIE);
            pSprite->SetPosition(rcPos.left + 15, 400);
            pSprite->SetVelocity(0, -7);
            g_pGame->AddSprite(pSprite);
            // Play the missile (fire) sound
            PlaySound((LPCSTR)IDW_MISSILE, g_hInstance, SND_ASYNC |
                      SND_RESOURCE | SND_NOSTOP);
            // Reset the input delay
            g_iFireInputDelay = 0;
        }
    }

    // Start a new game based upon an Enter (Return) key press
    if (GetAsyncKeyState(VK_RETURN) < 0)
        if (g_bDemo) {
            // Switch out of demo mode to start a new game
            g_bDemo = FALSE;
            NewGame();
        } else if (g_bGameOver) {
            // Start a new game
            NewGame();
        }
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
    // See if a player missile and an alien have collided
    Bitmap* pHitter = pSpriteHitter->GetBitmap();
    Bitmap* pHittee = pSpriteHittee->GetBitmap();

    if ((pHitter == g_pMissileBitmap && (pHittee == g_pBlobboBitmap ||
                                         pHittee == g_pJellyBitmap || pHittee == g_pTimmyBitmap)) ||
        (pHittee == g_pMissileBitmap && (pHitter == g_pBlobboBitmap ||
                                         pHitter == g_pJellyBitmap || pHitter == g_pTimmyBitmap))) {
        // Play the small explosion sound
        PlaySound((LPCSTR)IDW_LGEXPLODE, g_hInstance, SND_ASYNC |
                  SND_RESOURCE);
        // Kill both sprites
        pSpriteHitter->Kill();
        pSpriteHittee->Kill();
        // Create a large explosion sprite at the alien's position
        RECT rcBounds = { 0, 0, 600, 450 };
        RECT rcPos;

        if (pHitter == g_pMissileBitmap)
            rcPos = pSpriteHittee->GetPosition();
        else
            rcPos = pSpriteHitter->GetPosition();

        Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, rcBounds);
        pSprite->SetNumFrames(8, TRUE);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        g_pGame->AddSprite(pSprite);
        // Update the score
        g_iScore += 25;
        g_iDifficulty = max(80 - (g_iScore / 20), 20);
    }

    // See if an alien missile has collided with the car
    if ((pHitter == g_pCarBitmap && (pHittee == g_pBMissileBitmap ||
                                     pHittee == g_pJMissileBitmap || pHittee == g_pTMissileBitmap)) ||
        (pHittee == g_pCarBitmap && (pHitter == g_pBMissileBitmap ||
                                     pHitter == g_pJMissileBitmap || pHitter == g_pTMissileBitmap))) {
        // Play the large explosion sound
        PlaySound((LPCSTR)IDW_LGEXPLODE, g_hInstance, SND_ASYNC |
                  SND_RESOURCE);

        // Kill the missile sprite
        if (pHitter == g_pCarBitmap)
            pSpriteHittee->Kill();
        else
            pSpriteHitter->Kill();

        // Create a large explosion sprite at the car's position
        RECT rcBounds = { 0, 0, 600, 480 };
        RECT rcPos;

        if (pHitter == g_pCarBitmap)
            rcPos = pSpriteHitter->GetPosition();
        else
            rcPos = pSpriteHittee->GetPosition();

        Sprite* pSprite = new Sprite(g_pLgExplosionBitmap, rcBounds);
        pSprite->SetNumFrames(8, TRUE);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        g_pGame->AddSprite(pSprite);
        // Move the car back to the start
        g_pCarSprite->SetPosition(300, 405);

        // See if the game is over
        if (--g_iNumLives == 0) {
            // Play the game over sound
            PlaySound((LPCSTR)IDW_GAMEOVER, g_hInstance, SND_ASYNC |
                      SND_RESOURCE);
            g_bGameOver = TRUE;
            g_iGameOverDelay = 150;
            // Update the hi scores
            UpdateHiScores();
        }
    }

    return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
    // See if an alien missile sprite is dying
    if (pSpriteDying->GetBitmap() == g_pBMissileBitmap ||
        pSpriteDying->GetBitmap() == g_pJMissileBitmap ||
        pSpriteDying->GetBitmap() == g_pTMissileBitmap) {
        // Play the small explosion sound
        if (!g_bDemo)
            PlaySound((LPCSTR)IDW_SMEXPLODE, g_hInstance, SND_ASYNC |
                      SND_RESOURCE | SND_NOSTOP);

        // Create a small explosion sprite at the missile's position
        RECT rcBounds = { 0, 0, 600, 450 };
        RECT rcPos = pSpriteDying->GetPosition();
        Sprite* pSprite = new Sprite(g_pSmExplosionBitmap, rcBounds);
        pSprite->SetNumFrames(8, TRUE);
        pSprite->SetPosition(rcPos.left, rcPos.top);
        g_pGame->AddSprite(pSprite);
    }
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void NewGame()
{
    // Clear the sprites
    g_pGame->CleanupSprites();
    // Initialize the game variables
    g_iFireInputDelay = 0;
    g_iScore = 0;
    g_iNumLives = 3;
    g_iDifficulty = 80;
    g_bGameOver = FALSE;

    if (g_bDemo) {
        // Add a few aliens to the demo
        for (int i = 0; i < 6; i++)
            AddAlien();
    } else {
        // Create the car sprite
        RECT rcBounds = { 0, 0, 600, 450 };
        g_pCarSprite = new Sprite(g_pCarBitmap, rcBounds, BA_WRAP);
        g_pCarSprite->SetPosition(300, 405);
        g_pGame->AddSprite(g_pCarSprite);
        // Play the background music
        g_pGame->PlayMIDISong(TEXT("Music.mid"));
    }
}

void AddAlien()
{
    // Create a new random alien sprite
    RECT          rcBounds = { 0, 0, 600, 410 };
    AlienSprite*  pSprite;

    switch (rand() % 3) {
    case 0:
        // Blobbo
        pSprite = new AlienSprite(g_pBlobboBitmap, rcBounds, BA_BOUNCE);
        pSprite->SetNumFrames(8);
        pSprite->SetPosition(((rand() % 2) == 0) ? 0 : 600, rand() % 370);
        pSprite->SetVelocity((rand() % 7) - 2, (rand() % 7) - 2);
        break;

    case 1:
        // Jelly
        pSprite = new AlienSprite(g_pJellyBitmap, rcBounds, BA_BOUNCE);
        pSprite->SetNumFrames(8);
        pSprite->SetPosition(rand() % 600, rand() % 370);
        pSprite->SetVelocity((rand() % 5) - 2, (rand() % 5) + 3);
        break;

    case 2:
        // Timmy
        pSprite = new AlienSprite(g_pTimmyBitmap, rcBounds, BA_WRAP);
        pSprite->SetNumFrames(8);
        pSprite->SetPosition(rand() % 600, rand() % 370);
        pSprite->SetVelocity((rand() % 7) + 3, 0);
        break;
    }

    // Add the alien sprite
    g_pGame->AddSprite(pSprite);
}

void UpdateHiScores()
{
    // See if the current score made the hi score list
    int i;

    for (i = 0; i < 5; i++) {
        if (g_iScore > g_iHiScores[i])
            break;
    }

    // Insert the current score into the hi score list
    if (i < 5) {
        for (int j = 4; j > i; j--) {
            g_iHiScores[j] = g_iHiScores[j - 1];
        }

        g_iHiScores[i] = g_iScore;
    }
}

BOOL ReadHiScores()
{
    // Open the hi score file (HiScores.dat)
    HANDLE hFile = CreateFile(TEXT("HiScores.dat"), GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        // The hi score file doesn't exist, so initialize the scores to 0
        for (int i = 0; i < 5; i++)
            g_iHiScores[i] = 0;

        return FALSE;
    }

    // Read the scores
    for (int i = 0; i < 5; i++) {
        // Read the score
        char  cData[6];
        DWORD dwBytesRead;

        if (!ReadFile(hFile, &cData, 5, &dwBytesRead, NULL)) {
            // Something went wrong, so close the file handle
            CloseHandle(hFile);
            return FALSE;
        }

        // Extract each integer score from the score data
        g_iHiScores[i] = atoi(cData);
    }

    // Close the file
    return CloseHandle(hFile);
}

BOOL WriteHiScores()
{
    // Create the hi score file (HiScores.dat) for writing
    HANDLE hFile = CreateFile(TEXT("HiScores.dat"), GENERIC_WRITE, 0, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        // The hi score file couldn't be created, so bail
        return FALSE;

    // Write the scores
    for (int i = 0; i < 5; i++) {
        // Format each score for writing
        CHAR cData[6];
        wsprintf(cData, "%05d", g_iHiScores[i]);
        // Write the score
        DWORD dwBytesWritten;

        if (!WriteFile(hFile, &cData, 5, &dwBytesWritten, NULL)) {
            // Something went wrong, so close the file handle
            CloseHandle(hFile);
            return FALSE;
        }
    }

    // Close the file
    return CloseHandle(hFile);
}
