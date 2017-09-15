//-----------------------------------------------------------------
// Light Cycles Application
// C++ Source - LightCycles.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "LightCycles.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Light Cycles"),
                             TEXT("Light Cycles"), IDI_LIGHTCYCLES, IDI_LIGHTCYCLES_SM, 500, 400);

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
    // Create and load the background and light cycle bitmaps
    HDC hDC = GetDC(hWindow);
    g_pBackground = new Bitmap(hDC, IDB_BACKGROUND, g_hInstance);
    g_pCycle[0][0] = new Bitmap(hDC, IDB_CYCLEBLUE_0, g_hInstance);
    g_pCycle[0][1] = new Bitmap(hDC, IDB_CYCLEBLUE_90, g_hInstance);
    g_pCycle[0][2] = new Bitmap(hDC, IDB_CYCLEBLUE_180, g_hInstance);
    g_pCycle[0][3] = new Bitmap(hDC, IDB_CYCLEBLUE_270, g_hInstance);
    g_pCycle[1][0] = new Bitmap(hDC, IDB_CYCLEORANGE_0, g_hInstance);
    g_pCycle[1][1] = new Bitmap(hDC, IDB_CYCLEORANGE_90, g_hInstance);
    g_pCycle[1][2] = new Bitmap(hDC, IDB_CYCLEORANGE_180, g_hInstance);
    g_pCycle[1][3] = new Bitmap(hDC, IDB_CYCLEORANGE_270, g_hInstance);
    // Start a new game
    NewGame();
}

void GameEnd()
{
    // Cleanup the background and light cycle bitmaps
    delete g_pBackground;

    for (int i = 0; i < 4; i++) {
        delete g_pCycle[0][i];
        delete g_pCycle[1][i];
    }

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
    // Draw the background
    g_pBackground->Draw(hDC, 0, 0);

    // Draw the light cycle trails
    for (int i = 0; i < 2; i++) {
        // Create a blue/orange pen depending on which trail is being drawn
        HPEN hPen = CreatePen(PS_SOLID, 5, (i == 0) ? RGB(0, 0, 255) : RGB(255, 146, 73));
        SelectObject(hDC, hPen);
        // Start at the first point in the trail
        MoveToEx(hDC, g_ptCycleTrail[i][0].x, g_ptCycleTrail[i][0].y, NULL);

        // Draw a line to each of the remaining points
        for (int j = 1; j < g_iTrailLen[i]; j++)
            LineTo(hDC, g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j].y);

        // Clean up the pen
        DeleteObject(hPen);
    }

    // Determine the directions of the light cycles
    int iDirection[2] = { 0, 0 };

    for (int i = 0; i < 2; i++) {
        if (g_ptCycleSpeed[i].y < 0)
            iDirection[i] = 0;
        else if (g_ptCycleSpeed[i].x > 0)
            iDirection[i] = 1;
        else if (g_ptCycleSpeed[i].y > 0)
            iDirection[i] = 2;
        else if (g_ptCycleSpeed[i].x < 0)
            iDirection[i] = 3;
    }

    // Draw the light cycles
    g_pCycle[0][iDirection[0]]->Draw(hDC, g_ptCyclePos[0].x,
                                     g_ptCyclePos[0].y, TRUE);
    g_pCycle[1][iDirection[1]]->Draw(hDC, g_ptCyclePos[1].x,
                                     g_ptCyclePos[1].y, TRUE);
}

void GameCycle()
{
    if (!g_bGameOver) {
        // Move the light cycles
        UpdateCycles();
        // Force a repaint to redraw the light cycles
        InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
    }
}

void HandleKeys()
{
    if (!g_bGameOver) {
        // Steer the blue light cycle in response to arrow key presses
        if (GetAsyncKeyState(VK_UP) < 0)
            SteerCycle(0, 0);
        else if (GetAsyncKeyState(VK_RIGHT) < 0)
            SteerCycle(0, 1);
        else if (GetAsyncKeyState(VK_DOWN) < 0)
            SteerCycle(0, 2);
        else if (GetAsyncKeyState(VK_LEFT) < 0)
            SteerCycle(0, 3);
    } else if (GetAsyncKeyState(VK_RETURN) < 0)
        NewGame();
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
    if (!g_bGameOver) {
        // Steer the orange light cycle in response to joystick moves
        if (jsJoystickState & JOY_UP)
            SteerCycle(1, 0);
        else if (jsJoystickState & JOY_RIGHT)
            SteerCycle(1, 1);
        else if (jsJoystickState & JOY_DOWN)
            SteerCycle(1, 2);
        else if (jsJoystickState & JOY_LEFT)
            SteerCycle(1, 3);
    } else if (jsJoystickState & JOY_FIRE1)
        NewGame();
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void NewGame()
{
    // Set the initial blue light cycle position and speed
    g_ptCyclePos[0].x = 250 - (g_pCycle[0][0]->GetWidth() / 2);
    g_ptCyclePos[0].y = 400 - g_pCycle[0][0]->GetHeight();
    g_ptCycleSpeed[0].x = 0;
    g_ptCycleSpeed[0].y = -g_iSPEED;
    // Set the initial orange light cycle position and speed
    g_ptCyclePos[1].x = 250 - (g_pCycle[1][0]->GetWidth() / 2);
    g_ptCyclePos[1].y = 0;
    g_ptCycleSpeed[1].x = 0;
    g_ptCycleSpeed[1].y = g_iSPEED;
    // Set the light cycle trail lengths and initial points
    g_iTrailLen[0] = g_iTrailLen[1] = 2;
    g_ptCycleTrail[0][0].x = g_ptCycleTrail[0][1].x = 250;
    g_ptCycleTrail[0][0].y = g_ptCycleTrail[0][1].y = 400;
    g_ptCycleTrail[1][0].x = g_ptCycleTrail[1][1].x = 250;
    g_ptCycleTrail[1][0].y = g_ptCycleTrail[1][1].y = 0;
    // Start the game
    g_bGameOver = FALSE;
}

void UpdateCycles()
{
    for (int i = 0; i < 2; i++) {
        // Update the light cycle position based on its speed
        g_ptCyclePos[i].x = g_ptCyclePos[i].x + g_ptCycleSpeed[i].x;
        g_ptCyclePos[i].y = g_ptCyclePos[i].y + g_ptCycleSpeed[i].y;
        // Update the light cycle trail based on its new position
        g_ptCycleTrail[i][g_iTrailLen[i] - 1].x =
            g_ptCyclePos[i].x + (g_pCycle[i][0]->GetWidth() / 2);
        g_ptCycleTrail[i][g_iTrailLen[i] - 1].y =
            g_ptCyclePos[i].y + (g_pCycle[i][0]->GetHeight() / 2);

        // See if the light cycle ran into the edge of the screen
        if (g_ptCyclePos[i].x < 0 ||
            g_ptCyclePos[i].x > (500 - g_pCycle[i][0]->GetWidth()) ||
            g_ptCyclePos[i].y < 0 ||
            g_ptCyclePos[i].y > (400 - g_pCycle[i][0]->GetHeight())) {
            // The game is over
            EndGame(1 - i);
            return;
        }

        // See if the light cycle collided with its own trail
        RECT rcTmpTrail;

        if (g_iTrailLen[i] > 2) { // Must have steered at least once
            for (int j = 0; j < g_iTrailLen[i] - 2; j++) {
                rcTmpTrail.left = min(g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j + 1].x) - 1;
                rcTmpTrail.right = max(g_ptCycleTrail[i][j].x, g_ptCycleTrail[i][j + 1].x) + 1;
                rcTmpTrail.top = min(g_ptCycleTrail[i][j].y, g_ptCycleTrail[i][j + 1].y) - 1;
                rcTmpTrail.bottom = max(g_ptCycleTrail[i][j].y, g_ptCycleTrail[i][j + 1].y) + 1;

                if (PtInRect(&rcTmpTrail, g_ptCycleTrail[i][g_iTrailLen[i] - 1]) != 0) {
                    // The game is over
                    EndGame(1 - i);
                    return;
                }
            }
        }

        // See if the light cycle collided with the other cycle's trail
        for (int j = 0; j <= g_iTrailLen[1 - i] - 2; j++) {
            rcTmpTrail.left = min(g_ptCycleTrail[1 - i][j].x, g_ptCycleTrail[1 - i][j + 1].x) - 3;
            rcTmpTrail.right = max(g_ptCycleTrail[1 - i][j].x, g_ptCycleTrail[1 - i][j + 1].x) + 3;
            rcTmpTrail.top = min(g_ptCycleTrail[1 - i][j].y, g_ptCycleTrail[1 - i][j + 1].y) - 3;
            rcTmpTrail.bottom = max(g_ptCycleTrail[1 - i][j].y, g_ptCycleTrail[1 - i][j + 1].y) + 3;

            if (PtInRect(&rcTmpTrail, g_ptCycleTrail[i][g_iTrailLen[i] - 1]) != 0) {
                // The game is over
                EndGame(1 - i);
                return;
            }
        }
    }
}

void SteerCycle(int iCycle, int iDirection)
{
    // Remember the old light cycle speed
    POINT ptOldSpeed;
    ptOldSpeed.x = g_ptCycleSpeed[iCycle].x;
    ptOldSpeed.y = g_ptCycleSpeed[iCycle].y;

    // Change the speed of the light cycle to steer it
    switch (iDirection) {
    case 0: // Up (0 degrees)
        if (g_ptCycleSpeed[iCycle].y == 0) {
            g_ptCycleSpeed[iCycle].x = 0;
            g_ptCycleSpeed[iCycle].y = -g_iSPEED;
        }

        break;

    case 1: // Right (90 degrees)
        if (g_ptCycleSpeed[iCycle].x == 0) {
            g_ptCycleSpeed[iCycle].x = g_iSPEED;
            g_ptCycleSpeed[iCycle].y = 0;
        }

        break;

    case 2: // Down (180 degrees)
        if (g_ptCycleSpeed[iCycle].y == 0) {
            g_ptCycleSpeed[iCycle].x = 0;
            g_ptCycleSpeed[iCycle].y = g_iSPEED;
        }

        break;

    case 3: // Left (270 degrees)
        if (g_ptCycleSpeed[iCycle].x == 0) {
            g_ptCycleSpeed[iCycle].x = -g_iSPEED;
            g_ptCycleSpeed[iCycle].y = 0;
        }

        break;
    }

    // If the speed changed, move to a new point in the light cycle trail
    if ((g_ptCycleSpeed[iCycle].x != ptOldSpeed.x) ||
        (g_ptCycleSpeed[iCycle].y != ptOldSpeed.y)) {
        // Increment the number of trail points
        g_iTrailLen[iCycle]++;
        // Set the initial position of the new trail point
        g_ptCycleTrail[iCycle][g_iTrailLen[iCycle] - 1].x =
            g_ptCyclePos[iCycle].x + (g_pCycle[iCycle][0]->GetWidth() / 2);
        g_ptCycleTrail[iCycle][g_iTrailLen[iCycle] - 1].y =
            g_ptCyclePos[iCycle].y + (g_pCycle[iCycle][0]->GetHeight() / 2);
    }
}

void EndGame(int iCycle)
{
    // Set the game over flag
    g_bGameOver = TRUE;

    // Display a message about the winner
    if (iCycle == 0)
        MessageBox(g_pGame->GetWindow(), TEXT("Blue wins!"), TEXT("Light Cycles"), MB_OK);
    else
        MessageBox(g_pGame->GetWindow(), TEXT("Orange wins!"), TEXT("Light Cycles"), MB_OK);
}
