//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - GameEngine.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
GameEngine *GameEngine::m_pGameEngine = NULL;

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
    MSG         msg;
    static int  iTickTrigger = 0;
    int         iTickCount;

    if (GameInitialize(hInstance)) {
        // Initialize the game engine
        if (!GameEngine::GetEngine()->Initialize(iCmdShow))
            return FALSE;

        // Enter the main message loop
        while (TRUE) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                // Process the message
                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                // Make sure the game engine isn't sleeping
                if (!GameEngine::GetEngine()->GetSleep()) {
                    // Check the tick count to see if a game cycle has elapsed
                    iTickCount = GetTickCount();

                    if (iTickCount > iTickTrigger) {
                        iTickTrigger = iTickCount +
                                       GameEngine::GetEngine()->GetFrameDelay();
                        HandleKeys();
                        GameCycle();
                    }
                }
            }
        }

        return (int)msg.wParam;
    }

    // End the game
    GameEnd();
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Route all Windows messages to the game engine
    return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// GameEngine Constructor(s)/Destructor
//-----------------------------------------------------------------
GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
                       LPTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
    // Set the member variables for the game engine
    m_pGameEngine = this;
    m_hInstance = hInstance;
    m_hWindow = NULL;

    if (lstrlen(szWindowClass) > 0)
        lstrcpy(m_szWindowClass, szWindowClass);

    if (lstrlen(szTitle) > 0)
        lstrcpy(m_szTitle, szTitle);

    m_wIcon = wIcon;
    m_wSmallIcon = wSmallIcon;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iFrameDelay = 50;   // 20 FPS default
    m_bSleep = TRUE;
}

GameEngine::~GameEngine()
{
}

//-----------------------------------------------------------------
// Game Engine General Methods
//-----------------------------------------------------------------
BOOL GameEngine::Initialize(int iCmdShow)
{
    WNDCLASSEX    wndclass;
    // Create the window class for the main window
    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = m_hInstance;
    wndclass.hIcon          = LoadIcon(m_hInstance,
                                       MAKEINTRESOURCE(GetIcon()));
    wndclass.hIconSm        = LoadIcon(m_hInstance,
                                       MAKEINTRESOURCE(GetSmallIcon()));
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = m_szWindowClass;

    // Register the window class
    if (!RegisterClassEx(&wndclass))
        return FALSE;

    // Calculate the window size and position based upon the game size
    int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
        iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
                        GetSystemMetrics(SM_CYCAPTION);

    if (wndclass.lpszMenuName != NULL)
        iWindowHeight += GetSystemMetrics(SM_CYMENU);

    int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
        iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;
    // Create the window
    m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
                             WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
                             iWindowHeight, NULL, NULL, m_hInstance, NULL);

    if (!m_hWindow)
        return FALSE;

    // Show and update the window
    ShowWindow(m_hWindow, iCmdShow);
    UpdateWindow(m_hWindow);
    return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Route Windows messages to game engine member functions
    switch (msg) {
    case WM_CREATE:
        // Set the game window and start the game
        SetWindow(hWindow);
        GameStart(hWindow);
        return 0;

    case WM_SETFOCUS:
        // Activate the game and update the Sleep status
        GameActivate(hWindow);
        SetSleep(FALSE);
        return 0;

    case WM_KILLFOCUS:
        // Deactivate the game and update the Sleep status
        GameDeactivate(hWindow);
        SetSleep(TRUE);
        return 0;

    case WM_PAINT:
        HDC         hDC;
        PAINTSTRUCT ps;
        hDC = BeginPaint(hWindow, &ps);
        // Paint the game
        GamePaint(hDC);
        EndPaint(hWindow, &ps);
        return 0;

    case WM_LBUTTONDOWN:
        // Handle left mouse button press
        MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_LBUTTONUP:
        // Handle left mouse button release
        MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;

    case WM_RBUTTONDOWN:
        // Handle right mouse button press
        MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
        return 0;

    case WM_RBUTTONUP:
        // Handle right mouse button release
        MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
        return 0;

    case WM_MOUSEMOVE:
        // Handle mouse movement
        MouseMove(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_DESTROY:
        // End the game and exit the application
        GameEnd();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
    MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
    PostQuitMessage(0);
}
