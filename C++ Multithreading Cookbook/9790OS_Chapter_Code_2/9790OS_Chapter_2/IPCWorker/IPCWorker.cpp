#include "stdafx.h"
#include <Windows.h>

#define COMMUNICATION_OBJECT_NAME   TEXT("__FILE_MAPPING__")
#define SYNCHRONIZING_MUTEX_NAME    TEXT( "__TEST_MUTEX__" )

typedef struct _tagCOMMUNICATIONOBJECT {
    HWND    hWndClient;
    BOOL    bExitLoop;
    LONG    lSleepTimeout;
} COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

int _tmain(int argc, _TCHAR* argv[])
{
    HBRUSH hBrush = NULL;

    if (_tcscmp(TEXT("blue"), argv[0]) == 0) {
        hBrush = CreateSolidBrush(RGB(0, 0, 255));
    } else {
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
    }

    HWND hWnd = NULL;
    HDC hDC = NULL;
    RECT rectClient = { 0 };
    LONG lWaitTimeout = 0;
    HANDLE hMapping = NULL;
    PCOMMUNICATIONOBJECT pCommObject = NULL;
    BOOL bContinueLoop = TRUE;
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONIZING_MUTEX_NAME);
    hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, COMMUNICATION_OBJECT_NAME);

    if (hMapping) {
        while (bContinueLoop) {
            WaitForSingleObject(hMutex, INFINITE);
            pCommObject = (PCOMMUNICATIONOBJECT) MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, sizeof(COMMUNICATIONOBJECT));

            if (pCommObject) {
                bContinueLoop = !pCommObject->bExitLoop;
                hWnd = pCommObject->hWndClient;
                lWaitTimeout = pCommObject->lSleepTimeout;
                UnmapViewOfFile(pCommObject);
                hDC = GetDC(hWnd);

                if (GetClientRect(hWnd, &rectClient)) {
                    FillRect(hDC, &rectClient, hBrush);
                }

                ReleaseDC(hWnd, hDC);
                Sleep(lWaitTimeout);
            }

            ReleaseMutex(hMutex);
        }
    }

    CloseHandle(hMapping);
    CloseHandle(hMutex);
    DeleteObject(hBrush);
    return 0;
}
