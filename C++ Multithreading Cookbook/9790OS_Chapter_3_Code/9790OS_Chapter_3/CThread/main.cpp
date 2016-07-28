#include "CThread.h"

class Thread : public CThread
{
protected:
    virtual void Run(LPVOID lpParameter = 0);
};

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void Thread::Run(LPVOID lpParameter)
{
    WNDCLASSEX wndEx = { 0 };
    wndEx.cbClsExtra = 0;
    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.cbWndExtra = 0;
    wndEx.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndEx.hInstance = GetModuleHandle(NULL);
    wndEx.lpfnWndProc = WindowProcedure;
    wndEx.lpszClassName = (LPCTSTR) this->lpParameter;
    wndEx.lpszMenuName = NULL;
    wndEx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    if (!RegisterClassEx(&wndEx)) {
        return;
    }

    HWND hWnd = CreateWindow(wndEx.lpszClassName, TEXT("Basic Thread Management"), WS_OVERLAPPEDWINDOW, 200, 200, 800, 600, HWND_DESKTOP, NULL, wndEx.hInstance, NULL);
    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOW);
    MSG msg = { 0 };

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(wndEx.lpszClassName, wndEx.hInstance);
}

int main()
{
    Thread thread;
    thread.Create(TEXT("WND_CLASS_1"));
    Thread pthread;
    pthread.Create(TEXT("WND_CLASS_2"));
    thread.Join();
    pthread.Join();
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    case WM_CLOSE: {
        DestroyWindow(hWnd);
        break;
    }

    default: {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }

    return 0;
}
