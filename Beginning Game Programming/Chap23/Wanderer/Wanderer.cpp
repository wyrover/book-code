//-----------------------------------------------------------------
// Wanderer Application
// C++ Source - Wanderer.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Wanderer.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Wanderer"),
                             TEXT("Wanderer"), IDI_WANDERER, IDI_WANDERER_SM, 256, 256);

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
    // Create the offscreen device context and bitmap
    g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
                         g_pGame->GetWidth(), g_pGame->GetHeight());
    SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);
    // Create the scrolling background and landscape layer
    HDC hDC = GetDC(hWindow);
    g_pBackground = new ScrollingBackground(256, 256);
    g_pBGLandscapeLayer = new BackgroundLayer(hDC, IDB_BG_LANDSCAPE, g_hInstance);
    RECT rcViewport = { 352, 352, 608, 608 };
    g_pBGLandscapeLayer->SetViewport(rcViewport);
    g_pBackground->AddLayer(g_pBGLandscapeLayer);
    // Create the scrolling foreground and clouds layer
    g_pForeground = new ScrollingBackground(256, 256);
    g_pFGCloudsLayer = new BackgroundLayer(hDC, IDB_BG_CLOUDS, g_hInstance);
    rcViewport.left = rcViewport.top = 64;
    rcViewport.right = rcViewport.bottom = 320;
    g_pFGCloudsLayer->SetViewport(rcViewport);
    g_pForeground->AddLayer(g_pFGCloudsLayer);
    // Create and load the person bitmap
    g_pPersonBitmap = new Bitmap(hDC, IDB_PERSON, g_hInstance);
    // Create the person sprite
    RECT rcBounds = { 115, 112, 26, 32 };
    g_pPersonSprite = new PersonSprite(g_pPersonBitmap, rcBounds, BA_STOP);
    g_pPersonSprite->SetNumFrames(2);
    g_pPersonSprite->SetPosition(115, 112);
    g_pGame->AddSprite(g_pPersonSprite);
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
    delete g_pPersonBitmap;
    // Cleanup the scrolling background and landscape layer
    delete g_pBackground;
    delete g_pBGLandscapeLayer;
    // Cleanup the scrolling foreground and clouds layer
    delete g_pForeground;
    delete g_pFGCloudsLayer;
    // Cleanup the sprites
    g_pGame->CleanupSprites();
    // Cleanup the game engine
    delete g_pGame;
}

void GameActivate(HWND hWindow)
{
    // Capture the joystick
    g_pGame->CaptureJoystick();
    // Resume the background music
    g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    // Release the joystick
    g_pGame->ReleaseJoystick();
    // Pause the background music
    g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the scrolling background
    g_pBackground->Draw(hDC);
    // Draw the sprites
    g_pGame->DrawSprites(hDC);
    // Draw the scrolling foreground
    g_pForeground->Draw(hDC, TRUE); // draw with transparency
}

void GameCycle()
{
    // Update the background
//  g_pBackground->Update();
    // Update the foreground
//  g_pForeground->Update();
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
    // Move the landscape/cloud layers based upon arrow key presses
    if (g_iInputDelay++ > 1) {
        if (GetAsyncKeyState(VK_LEFT) < 0) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer to the right
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_RIGHT);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer to the right
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_RIGHT);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (GetAsyncKeyState(VK_RIGHT) < 0) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer to the left
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_LEFT);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer to the left
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_LEFT);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (GetAsyncKeyState(VK_UP) < 0) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer down
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_DOWN);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer down
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_DOWN);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (GetAsyncKeyState(VK_DOWN) < 0) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer up
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_UP);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer up
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_UP);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        }

        // Reset the input delay
        g_iInputDelay = 0;
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
    if (++g_iInputDelay > 2) {
        // Check horizontal movement
        if (jsJoystickState & JOY_LEFT) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer to the right
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_RIGHT);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer to the right
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_RIGHT);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (jsJoystickState & JOY_RIGHT) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer to the left
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_LEFT);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer to the left
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_LEFT);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (jsJoystickState & JOY_UP) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer down
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_DOWN);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer down
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_DOWN);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        } else if (jsJoystickState & JOY_DOWN) {
            // Make the person walk
            g_pPersonSprite->Walk();
            // Move the landscape layer up
            g_pBGLandscapeLayer->SetSpeed(16);
            g_pBGLandscapeLayer->SetDirection(SD_UP);
            g_pBGLandscapeLayer->Update();
            g_pBGLandscapeLayer->SetSpeed(0);
            // Move the cloud layer up
            g_pFGCloudsLayer->SetSpeed(4);
            g_pFGCloudsLayer->SetDirection(SD_UP);
            g_pFGCloudsLayer->Update();
            g_pFGCloudsLayer->SetSpeed(0);
        }

        // Reset the input delay
        g_iInputDelay = 0;
    }
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
}
