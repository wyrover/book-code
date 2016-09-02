// Win32_EventHandling.cpp : Implementation file
///////////////////////////////////////////////////////
//
// File           : Win32_EventHandling.cpp
// Version        : 2.0
// Function       :
//
// Author         : Mitendra Anand
// Date           : 03/11/2011
//
// Notes          :
//
// Modifications  :
//
//
///////////////////////////////////////////////////////

#include <windows.h>
#include <iostream>

using namespace std;


DWORD MyWaitForMultipleObjects(DWORD _NumHandles, HANDLE* _Handles, BOOL bWaitAll = TRUE, DWORD dwTimeoutMS = INFINITE)
{
    HRESULT hr = S_OK;
    DWORD dwWaitResult = WAIT_FAILED;

    if ((_NumHandles > MAXIMUM_WAIT_OBJECTS) && !bWaitAll) {
        cout << "MyWaitForMultipleObjects: WARNING: too many objects" << _NumHandles << MAXIMUM_WAIT_OBJECTS << endl;
        return dwWaitResult;
    }

    // Create tracking array
    // Handles will be removed as they are signaled
    DWORD       NumHandles  = _NumHandles;
    HANDLE*     Handles     = new HANDLE [_NumHandles + 1];

    for (DWORD i = 0; i < NumHandles; i++) {
        Handles[i]   = _Handles[i];
    }

    bool bDone      = false;
    bool bTimeout   = false;

    do {
        dwWaitResult = WaitForMultipleObjects(min(NumHandles, (MAXIMUM_WAIT_OBJECTS)), Handles, /*bWaitAll*/false, INFINITE);

        if (dwWaitResult == WAIT_FAILED) {
            cout << "WAIT_FAILED(MyWaitForMultipleObjects)" << endl;
        } else if (dwWaitResult == WAIT_TIMEOUT) {
            //timeout - we're done
            bTimeout = true;
        } else if ((dwWaitResult >= WAIT_OBJECT_0) && (dwWaitResult <= (WAIT_OBJECT_0 + NumHandles - 1))) {
            // A Handle has signaled
            if (bWaitAll) {
                // remove the handle from the array
                DWORD Index = dwWaitResult - WAIT_OBJECT_0;
                memmove(&Handles[Index],   &Handles[Index + 1], (NumHandles - Index)*sizeof(HANDLE));
                Handles[NumHandles - 1]      = 0;
                NumHandles--;

                if (NumHandles == 0) {
                    bDone = true;
                    dwWaitResult = WAIT_OBJECT_0;
                }
            } else {
                // we are done since bWaitAll is not set
                bDone = true;
            }
        }
    } while (!bDone && SUCCEEDED(hr));

    delete [] Handles;
    return dwWaitResult;
}

DWORD WINAPI ThreadFun1(LPVOID n)
{
    cout << "Thread Instantiated 1........." << endl;
    // Get the handler to the event for which we need to wait in
    //  this thread.
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS , false, L"MyEvent1");

    if (!hEvent) {
        return -1;
    }

    Sleep(5000);
    ResetEvent(hEvent);

    // Signal the event
    if (SetEvent(hEvent)) {
        cout << "Got The signal - MyEvent 1......." << endl;
    }

    CloseHandle(hEvent);
    cout << "End of the Thread 1......" << endl;
    return 0;
}

DWORD WINAPI ThreadFun2(LPVOID n)
{
    cout << "Thread Instantiated 2........." << endl;
    // Get the handler to the event for which we need to wait in
    //  this thread.
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS , false, L"MyEvent2");

    if (!hEvent) {
        return -1;
    }

    Sleep(10000);
    ResetEvent(hEvent);

    // Signal the event
    if (SetEvent(hEvent)) {
        cout << "Got The signal - MyEvent 2......." << endl;
    }

    CloseHandle(hEvent);
    cout << "End of the Thread 2......" << endl;
    return 0;
}

DWORD WINAPI ThreadFun3(LPVOID n)
{
    cout << "Thread Instantiated 3........." << endl;
    // Get the handler to the event for which we need to wait in
    //  this thread.
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS , false, L"MyEvent3");

    if (!hEvent) {
        return -1;
    }

    Sleep(15000);
    ResetEvent(hEvent);

    // Signal the event
    if (SetEvent(hEvent)) {
        cout << "Got The signal - MyEvent3......." << endl;
    }

    CloseHandle(hEvent);
    //for(;;);
    cout << "End of the Thread 3......" << endl;
    return 0;
}
int main()
{
    DWORD dwRet;
    // Aray to store thread handles
    HANDLE Array_Of_Events_Handles[3];
    //  Create an Manual Reset Event where events must be reset
    //  manually to non signalled state
    HANDLE  hEvent1 = CreateEvent(NULL , true , false , L"MyEvent1");

    if (!hEvent1) return -1;

    HANDLE  hEvent2 = CreateEvent(NULL , true , false , L"MyEvent2");

    if (!hEvent2) return -1;

    HANDLE  hEvent3 = CreateEvent(NULL , true , false , L"MyEvent3");

    if (!hEvent3) return -1;

    //  Create a Thread Which will wait for the events to occur
    DWORD Id;
    HANDLE hThrd1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun1, 0, 0, &Id);

    if (!hThrd1) {
        CloseHandle(hEvent1);
        return -1;
    }

    HANDLE hThrd2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun2, 0, 0, &Id);

    if (!hThrd2) {
        CloseHandle(hEvent2);
        return -1;
    }

    HANDLE hThrd3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFun3, 0, 0, &Id);

    if (!hThrd3) {
        CloseHandle(hEvent3);
        return -1;
    }

    // Wait for a while before continuing....
    Sleep(1000);
    Array_Of_Events_Handles[0] = hEvent1;//Thrd1;
    Array_Of_Events_Handles[1] = hEvent2;//Thrd2;
    Array_Of_Events_Handles[2] = hEvent3;//Thrd3;

    // Wait until all threads have terminated.
    while (1) {
        //dwRet = WaitForMultipleObjects( 3, Array_Of_Events_Handles, TRUE, 20000);
        dwRet = MyWaitForMultipleObjects(3, Array_Of_Events_Handles, TRUE, 20000);

        if ((dwRet >= WAIT_OBJECT_0) && dwRet <= (WAIT_OBJECT_0 + 2)) {
            // TODO: Perform tasks required by this event
            wprintf(L"All events were signaled...\n");
            break;
        }

        switch (dwRet) {
        //case WAIT_OBJECT_0 + 0:
        //// TODO: Perform tasks required by this event
        //wprintf(L"First event was signaled...\n");
        //break;

        //case WAIT_OBJECT_0 + 1:
        //// TODO: Perform tasks required by this event
        //wprintf(L"Second event was signaled...\n");
        //break;

        //case WAIT_OBJECT_0 + 2:
        //// TODO: Perform tasks required by this event
        //wprintf(L"Third event was signaled...\n");
        //break;

        // Time out
        case WAIT_TIMEOUT:
            wprintf(L"\nThe waiting is timed out...\n");
            break;

        // Return value is invalid.
        default:
            wprintf(L"\nWaiting failed, error %d...\n", GetLastError());
            ExitProcess(0);
        }
    }

    printf("\nlets close threads handles \n");
    CloseHandle(hThrd1);
    CloseHandle(hEvent1);
    CloseHandle(hThrd2);
    CloseHandle(hEvent2);
    CloseHandle(hThrd3);
    CloseHandle(hEvent3);
    cout << "End of Main ........" << endl;
    return 0;
}


