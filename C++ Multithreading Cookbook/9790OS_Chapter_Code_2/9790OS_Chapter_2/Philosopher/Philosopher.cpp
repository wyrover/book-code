#include "stdafx.h"
#include <Windows.h>

#define EATING_TIME         1000
#define PHILOSOPHER_COUNT   5
#define WM_INVALIDATE       WM_USER + 1

typedef struct _tagCOMMUNICATIONOBJECT {
    HWND    hWnd;
    bool    bExitApplication;
    int     iPhilosopherArray[PHILOSOPHER_COUNT];
    int     PhilosopherCount;
} COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

void Eat();
TCHAR* szSemaphoreName = TEXT("__PD_SEMAPHORE__");
TCHAR* szMappingName = TEXT("__SHARED_FILE_MAPPING__");
bool bExitApplication = false;

int _tmain(int argc, _TCHAR* argv[])
{
    HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_HIDE);
    int iIndex = (int)_tcstol(argv[0], NULL, 10);
    HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szMappingName);

    while (!bExitApplication) {
        HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, szSemaphoreName);
        WaitForSingleObject(hSemaphore, INFINITE);
        PCOMMUNICATIONOBJECT pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping,
                                           FILE_MAP_ALL_ACCESS, 0, 0, sizeof(COMMUNICATIONOBJECT));
        bExitApplication = pCommObject->bExitApplication;

        if (!pCommObject->iPhilosopherArray[(iIndex + pCommObject->PhilosopherCount - 1) % pCommObject->PhilosopherCount]
            && !pCommObject->iPhilosopherArray[(iIndex + 1) % pCommObject->PhilosopherCount]) {
            pCommObject->iPhilosopherArray[iIndex] = 1;
            Eat();
        }

        SendMessage(pCommObject->hWnd, WM_INVALIDATE, 0, 0);
        pCommObject->iPhilosopherArray[iIndex] = 0;
        UnmapViewOfFile(pCommObject);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        CloseHandle(hSemaphore);
    }

    CloseHandle(hMapping);
    return 0;
}

void Eat()
{
    Sleep(EATING_TIME);
}
