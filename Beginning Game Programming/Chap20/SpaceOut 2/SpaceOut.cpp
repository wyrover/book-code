//-----------------------------------------------------------------
// Space Out 2 Application
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
    g_pGame = new GameEngine(hInstance, TEXT("Space Out 2"),
                             TEXT("Space Out 2"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450);

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
    // Set the splash screen variable
    g_bSplash = TRUE;
    g_bGameOver = TRUE;
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
}

void GameActivate(HWND hWindow)
{
    if (!g_bSplash)
        // Resume the background music
        g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    if (!g_bSplash)
        // Pause the background music
        g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background
    g_pBackground->Draw(hDC);
    // Draw the desert bitmap
    g_pDesertBitmap->Draw(hDC, 0, 371);

    if (g_bSplash) {
        // Draw the splash screen image
        g_pSplashBitmap->Draw(hDC, 142, 100, TRUE);
    } else {
        // Draw the sprites
        g_pGame->DrawSprites(hDC);
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
        // Randomly add aliens
        if ((rand() % g_iDifficulty) == 0)
            AddAlien();

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
}

void HandleKeys()
{
    if (!g_bGameOver) {
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
        if (g_bSplash) {
            // Start a new game without the splash screen
            g_bSplash = FALSE;
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
    // Create the car sprite
    RECT rcBounds = { 0, 0, 600, 450 };
    g_pCarSprite = new Sprite(g_pCarBitmap, rcBounds, BA_WRAP);
    g_pCarSprite->SetPosition(300, 405);
    g_pGame->AddSprite(g_pCarSprite);
    // Play the background music
    g_pGame->PlayMIDISong(TEXT("Music.mid"));
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
