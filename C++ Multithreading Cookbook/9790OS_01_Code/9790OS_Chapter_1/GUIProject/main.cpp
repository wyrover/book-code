#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrev, LPSTR szCmdLine, int iCmdShow)
{
    UNREFERENCED_PARAMETER(hPrev);
    UNREFERENCED_PARAMETER(szCmdLine);
    WNDCLASSEX wndEx = { 0 };
    wndEx.cbClsExtra = 0;
    wndEx.cbSize = sizeof(wndEx);
    wndEx.cbWndExtra = 0;
    wndEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndEx.hInstance = hThis;
    wndEx.lpfnWndProc = WndProc;
    wndEx.lpszClassName = TEXT("GUIProject");
    wndEx.lpszMenuName = NULL;
    wndEx.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wndEx)) {
        return -1;
    }

    HWND hWnd = CreateWindow(wndEx.lpszClassName, TEXT("GUI Project"), WS_OVERLAPPEDWINDOW, 200, 200, 400, 300, HWND_DESKTOP, NULL, hThis, 0);

    if (!hWnd) {
        return -1;
    }

    UpdateWindow(hWnd);
    ShowWindow(hWnd, iCmdShow);
    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, NULL, NULL)) {
        // Translates virtual-key messages into character messages.
        TranslateMessage(&msg);
        // Dispatches a message to a window procedure.
        DispatchMessage(&msg);
    }

    DestroyWindow(hWnd);
    UnregisterClass(wndEx.lpszClassName, hThis);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CLOSE: {
        PostQuitMessage(0);
        break;
    }

    default: {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }

    return 0;
}

