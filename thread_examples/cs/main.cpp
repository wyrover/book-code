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

CRITICAL_SECTION cs;
int a[ 5 ];

void Thread(void* pParams)
{
    int i, num = 0;

    while (TRUE) {
        EnterCriticalSection(&cs);

        for (i = 0; i < 5; i++) a[ i ] = num;

        LeaveCriticalSection(&cs);
        num++;
    }
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    InitializeCriticalSection(&cs);
    _beginthread(Thread, 0, NULL);

    while (TRUE) {
        EnterCriticalSection(&cs);
        printf("%d %d %d %d %d\n",
               a[ 0 ], a[ 1 ], a[ 2 ],
               a[ 3 ], a[ 4 ]);
        LeaveCriticalSection(&cs);
    }

    return 0;
}

