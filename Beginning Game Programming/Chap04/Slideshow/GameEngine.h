//-----------------------------------------------------------------
// Game Engine Object
// C++ Header - GameEngine.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>

//-----------------------------------------------------------------
// Windows Function Declarations
//-----------------------------------------------------------------
int WINAPI        WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                          PSTR szCmdLine, int iCmdShow);
LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------
// Game Engine Function Declarations
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd();
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle();

//-----------------------------------------------------------------
// GameEngine Class
//-----------------------------------------------------------------
class GameEngine
{
protected:
    // Member Variables
    static GameEngine*  m_pGameEngine;
    HINSTANCE           m_hInstance;
    HWND                m_hWindow;
    TCHAR               m_szWindowClass[32];
    TCHAR               m_szTitle[32];
    WORD                m_wIcon, m_wSmallIcon;
    int                 m_iWidth, m_iHeight;
    int                 m_iFrameDelay;
    BOOL                m_bSleep;

public:
    // Constructor(s)/Destructor
    GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
               WORD wIcon, WORD wSmallIcon, int iWidth = 640, int iHeight = 480);
    virtual ~GameEngine();

    // General Methods
    static GameEngine*  GetEngine()
    {
        return m_pGameEngine;
    };
    BOOL                Initialize(int iCmdShow);
    LRESULT             HandleEvent(HWND hWindow, UINT msg, WPARAM wParam,
                                    LPARAM lParam);
    void                ErrorQuit(LPTSTR szErrorMsg);

    // Accessor Methods
    HINSTANCE GetInstance()
    {
        return m_hInstance;
    };
    HWND      GetWindow()
    {
        return m_hWindow;
    };
    void      SetWindow(HWND hWindow)
    {
        m_hWindow = hWindow;
    };
    LPTSTR    GetTitle()
    {
        return m_szTitle;
    };
    WORD      GetIcon()
    {
        return m_wIcon;
    };
    WORD      GetSmallIcon()
    {
        return m_wSmallIcon;
    };
    int       GetWidth()
    {
        return m_iWidth;
    };
    int       GetHeight()
    {
        return m_iHeight;
    };
    int       GetFrameDelay()
    {
        return m_iFrameDelay;
    };
    void      SetFrameRate(int iFrameRate)
    {
        m_iFrameDelay = 1000 /
                        iFrameRate;
    };
    BOOL      GetSleep()
    {
        return m_bSleep;
    };
    void      SetSleep(BOOL bSleep)
    {
        m_bSleep = bSleep;
    };
};
