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

//  User Defines Messages
#define     THRD_MESSAGE_SOMEWORK       WM_USER + 1
#define     THRD_MESSAGE_EXIT           WM_USER + 2
//  Application Specific Preprocessor definitions
#define     NUM_THREADS                 2

DWORD WINAPI    ThrdFunc(LPVOID n)
{
    int TNumber = (int)n;

    //  Here we will wait for the messages
    while (1) {
        MSG msg;
        //BOOL  MsgReturn  =  PeekMessage ( &msg , NULL ,
        //  THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL    MsgReturn   =   GetMessage(&msg , NULL ,
                                           THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT);

        if (MsgReturn) {
            switch (msg.message) {
            case THRD_MESSAGE_SOMEWORK:
                cout << "Working Message.... for Thread Number " << TNumber << endl;
                break;

            case THRD_MESSAGE_EXIT:
                cout << "Exiting Message... for Thread Number " << TNumber << endl;
                return 0;
            }
        }
    }

    return 0;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    HANDLE  hThrd [ NUM_THREADS ];
    DWORD   Id [ NUM_THREADS ];
    short LoopCounter = 0;

    //  Create all the threads
    for (; LoopCounter < NUM_THREADS ; LoopCounter ++) {
        hThrd [ LoopCounter ] = CreateThread(NULL , 0 , (LPTHREAD_START_ROUTINE)ThrdFunc ,
                                             (LPVOID)LoopCounter , 0, &Id [ LoopCounter ]);

        if (!hThrd) {
            cout << "Error Creating Threads,,,,.exiting" << endl;
            return -1;
        }

        Sleep(100);
    }

    Sleep(10000);

    //  Send Working Message to all Threads
    for (LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++) {
        PostThreadMessage(Id [ LoopCounter ] , THRD_MESSAGE_SOMEWORK , 0 , 0);
        Sleep(100);
    }

    //  Sleep againg for 1 seconds and send exit message to all threads
    Sleep(1000);

    for (LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++) {
        PostThreadMessage(Id [ LoopCounter ] , THRD_MESSAGE_EXIT, 0 , 0);
        Sleep(100);
    }

    //  Wait for all threads to exit
    WaitForMultipleObjects(NUM_THREADS, hThrd , true , INFINITE);

    //  Close All handles
    for (LoopCounter = 0; LoopCounter < NUM_THREADS ; LoopCounter ++)
        CloseHandle(hThrd [ LoopCounter ]);

    return 0;
}

