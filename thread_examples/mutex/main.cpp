// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

using namespace std;

HANDLE hMutex;
int a[ 5 ];

void Thread(void* pParams)
{
    int i, num = 0;

    while (TRUE) {
        WaitForSingleObject(hMutex, INFINITE);

        for (i = 0; i < 5; i++) a[ i ] = num;

        ReleaseMutex(hMutex);
        num++;
    }
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    hMutex = CreateMutex(NULL, FALSE, NULL);
    _beginthread(Thread, 0, NULL);

    while (TRUE) {
        WaitForSingleObject(hMutex, INFINITE);
        printf("%d %d %d %d %d\n",
               a[ 0 ], a[ 1 ], a[ 2 ],
               a[ 3 ], a[ 4 ]);
        ReleaseMutex(hMutex);
    }

    return 0;
}

