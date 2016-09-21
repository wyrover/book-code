// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

using namespace std;

DWORD WINAPI Tf(LPVOID n)
{
    cout << "Thread Instantiated........." << endl;
    // Get the handler to the event for which we need to wait in
    //  this thread.
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS , false, L"MyEvent");

    if (!hEvent) {
        return -1;
    }

    // Loop through and wait for an event to occur
    for (char counter = 0; counter < 2; counter ++) {
        // Wait for the Event
        WaitForSingleObject(hEvent, INFINITE);
        //  No need to Reset the event as its become non signalled as soon as
        //  some thread catches the event.
        cout << "Got The signal......." << endl;
    }

    CloseHandle(hEvent);
    cout << "End of the Thread......" << endl;
    return 0;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    //  Create an Auto Reset Event which automatically reset to
    //  Non Signalled state after being signalled
    HANDLE  hEvent = CreateEvent(NULL , false , false , L"MyEvent");

    if (!hEvent) return -1;

    //  Create a Thread Which will wait for the events to occur
    DWORD Id;
    HANDLE hThrd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Tf, 0, 0, &Id);

    if (!hThrd) {
        CloseHandle(hEvent);
        return -1;
    }

    // Wait for a while before continuing....
    Sleep(1000);

    // Give the signal twice as the thread is waiting for 2 signals to occur
    for (char counter = 0; counter < 2; counter ++) {
        // Signal the event
        SetEvent(hEvent);
        // wait for some time before giving another signal
        Sleep(2000);
    }

    // Wait for the Thread to Die
    WaitForSingleObject(hThrd, INFINITE);
    CloseHandle(hThrd);
    CloseHandle(hEvent);
    cout << "End of Main ........" << endl;
    return 0;
}

