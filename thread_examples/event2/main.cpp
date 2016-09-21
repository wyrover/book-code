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

HANDLE hEvent1, hEvent2;
int a[ 5 ];

void Thread(void* pParams)
{
    int i, num = 0;

    while (TRUE) {
        WaitForSingleObject(hEvent2, INFINITE);

        for (i = 0; i < 5; i++) a[ i ] = num;

        SetEvent(hEvent1);
        num++;
    }
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);
    _beginthread(Thread, 0, NULL);

    while (TRUE) {
        WaitForSingleObject(hEvent1, INFINITE);
        printf("%d %d %d %d %d\n",
               a[ 0 ], a[ 1 ], a[ 2 ],
               a[ 3 ], a[ 4 ]);
        SetEvent(hEvent2);
    }

    return 0;
}

