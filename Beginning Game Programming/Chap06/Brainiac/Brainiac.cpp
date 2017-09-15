//-----------------------------------------------------------------
// Brainiac Application
// C++ Source - Brainiac.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Brainiac.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Brainiac"),
                             TEXT("Brainiac"), IDI_BRAINIAC, IDI_BRAINIAC_SM, 528, 512);

    if (g_pGame == NULL)
        return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(1);
    // Store the instance handle
    g_hInstance = hInstance;
    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Seed the random number generator
    srand(GetTickCount());
    // Create and load the tile bitmaps
    HDC hDC = GetDC(hWindow);
    g_pTiles[0] = new Bitmap(hDC, IDB_TILEBLANK, g_hInstance);
    g_pTiles[1] = new Bitmap(hDC, IDB_TILE1, g_hInstance);
    g_pTiles[2] = new Bitmap(hDC, IDB_TILE2, g_hInstance);
    g_pTiles[3] = new Bitmap(hDC, IDB_TILE3, g_hInstance);
    g_pTiles[4] = new Bitmap(hDC, IDB_TILE4, g_hInstance);
    g_pTiles[5] = new Bitmap(hDC, IDB_TILE5, g_hInstance);
    g_pTiles[6] = new Bitmap(hDC, IDB_TILE6, g_hInstance);
    g_pTiles[7] = new Bitmap(hDC, IDB_TILE7, g_hInstance);
    g_pTiles[8] = new Bitmap(hDC, IDB_TILE8, g_hInstance);

    // Clear the tile states and images
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            g_bTileStates[i][j] = FALSE;
            g_iTiles[i][j] = 0;
        }

    // Initialize the tile images randomly
    for (int i = 0; i < 2; i++)
        for (int j = 1; j < 9; j++) {
            int x = rand() % 4;
            int y = rand() % 4;

            while (g_iTiles[x][y] != 0) {
                x = rand() % 4;
                y = rand() % 4;
            }

            g_iTiles[x][y] = j;
        }

    // Initialize the tile selections and match/try count
    g_ptTile1.x = g_ptTile1.y = -1;
    g_ptTile2.x = g_ptTile2.y = -1;
    g_iMatches = g_iTries = 0;
}

void GameEnd()
{
    // Cleanup the tile bitmaps
    for (int i = 0; i < 9; i++)
        delete g_pTiles[i];

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
    // Draw the tiles
    int iTileWidth = g_pTiles[0]->GetWidth();
    int iTileHeight = g_pTiles[0]->GetHeight();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (g_bTileStates[i][j] || ((i == g_ptTile1.x) && (j == g_ptTile1.y)) ||
                ((i == g_ptTile2.x) && (j == g_ptTile2.y)))
                g_pTiles[g_iTiles[i][j]]->Draw(hDC, i * iTileWidth, j * iTileHeight,
                                               TRUE);
            else
                g_pTiles[0]->Draw(hDC, i * iTileWidth, j * iTileHeight, TRUE);
}

void GameCycle()
{
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
    // Determine which tile was clicked
    int iTileX = x / g_pTiles[0]->GetWidth();
    int iTileY = y / g_pTiles[0]->GetHeight();

    // Make sure the tile hasn't already been matched
    if (!g_bTileStates[iTileX][iTileY]) {
        // See if this is the first tile selected
        if (g_ptTile1.x == -1) {
            // Set the first tile selection
            g_ptTile1.x = iTileX;
            g_ptTile1.y = iTileY;
        } else if ((iTileX != g_ptTile1.x) || (iTileY != g_ptTile1.y)) {
            if (g_ptTile2.x == -1) {
                // Increase the number of tries
                g_iTries++;
                // Set the second tile selection
                g_ptTile2.x = iTileX;
                g_ptTile2.y = iTileY;

                // See if it's a match
                if (g_iTiles[g_ptTile1.x][g_ptTile1.y] == g_iTiles[g_ptTile2.x][g_ptTile2.y]) {
                    // Set the tile state to indicate the match
                    g_bTileStates[g_ptTile1.x][g_ptTile1.y] = TRUE;
                    g_bTileStates[g_ptTile2.x][g_ptTile2.y] = TRUE;
                    // Clear the tile selections
                    g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;

                    // Update the match count and check for winner
                    if (++g_iMatches == 8) {
                        TCHAR szText[64];
                        wsprintf(szText, "You won in %d tries.", g_iTries);
                        MessageBox(g_pGame->GetWindow(), szText, TEXT("Brainiac"), MB_OK);
                    }
                }
            } else {
                // Clear the tile selections
                g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;
            }
        }

        // Force a repaint to update the tile
        InvalidateRect(g_pGame->GetWindow(), NULL, FALSE);
    }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}
