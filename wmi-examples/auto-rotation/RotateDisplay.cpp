#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

#include "targetver.h"
#include "resource.h"
#include "ScreenMode.h"

// This is just a quick hack to give us a background window and a tray icon to shut down.
// For the actual logic, see ScreenMode.cpp.
#define WM_TRAY_ICON (WM_USER + 1)
#define IDM_EXIT 0

class Window
{
private:
    NOTIFYICONDATA trayIcon;
    HINSTANCE hInstance;
    UINT_PTR rotationCheckTimer;

public:
    static ATOM RegisterWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEX wcex = {0};
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = Window::WndProc;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = "RotateDisplay";

        return RegisterClassEx(&wcex);
    }

    Window(HINSTANCE hInstance)
    {
        RegisterWindowClass(hInstance);

        this->hInstance = hInstance;
        CreateWindow("RotateDisplay", "Auto Rotate Display", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, this);
    }

    LRESULT CALLBACK RunWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        {
            trayIcon.cbSize = sizeof(NOTIFYICONDATA);
            trayIcon.hWnd = hWnd;
            trayIcon.uVersion = 4;
            trayIcon.uID = 1;
            trayIcon.uCallbackMessage = WM_TRAY_ICON;
            trayIcon.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
            trayIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; //| NIF_SHOWTIP;
            strcpy(trayIcon.szTip, "Auto Rotate Display");

            Shell_NotifyIcon(NIM_ADD, &trayIcon);
            Shell_NotifyIcon(NIM_SETVERSION, &trayIcon);

            rotationCheckTimer = SetTimer(hWnd, NULL, 500, NULL);

            return 0;
        }
        case WM_DESTROY:
        {
            KillTimer(hWnd, rotationCheckTimer);
            Shell_NotifyIcon(NIM_DELETE, &trayIcon);
            ResetOrientations();
            PostQuitMessage(0);
            return 0;
        }
        case WM_TRAY_ICON:
        {
            if(LOWORD(lParam) == WM_MOUSEMOVE)
                return 0;

            if(LOWORD(lParam) == WM_CONTEXTMENU)
            {
                HMENU hMenu = CreatePopupMenu();
                if(hMenu == NULL)
                    return 0;

                InsertMenu(hMenu, -1, MF_STRING, IDM_EXIT, "Close");

                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hWnd);
                TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                DestroyMenu(hMenu);
                PostMessage(hWnd, WM_NULL, 0, 0);
            }

            return 0;
        }
        case WM_TIMER:
            // All of the rest is just boilerplate for this.  Periodically check the screen's
            // orientation.
            SetOrientations();
            break;
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            switch(wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                return 0;
            }
            break;
        }

        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if(message == WM_CREATE)
        {
            CREATESTRUCT *createStruct = (CREATESTRUCT *)lParam;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
        }

        Window *window = (Window *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        return window->RunWndProc(hWnd, message, wParam, lParam);
    }
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    Window *window = new Window(hInstance);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

/*
 * Copyright (C) 2014 Glenn Maynard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
