/*
==========================================================================

Purpose:

This is a sample code that demonstrates for the following:

* Use of the I/O Completion ports with WinSock.  The idea is to create a
  simple application that will use IOCP, highlight how to use IOCP.

Notes:

* The server will create IOCP, Worker threads, all incoming client sockets
  will be associated with IOCP, the server will accept the client sent
  message will display it and then send the same message back as an
  acknowledgement.

Author:

* Swarajya Pendharkar

Date:

* 10th March 2006

Updates:

* Implemented IOCP with Overlapped I/O - 24th March 2006
* More updates pertaining to Overlapped I/O - 19th April 2006
* Updates pertaining to IOCP implementation and proper shutdown
  of application - 28th May 2006
* Minor tweaks and comments - 9th June 2006
* Updates to allow clients to send multiple messages and other
  code improvements - 19th August 2006
* Minor updates - 22th August 2006
* Number of worker threads will be based on the number of processors.
  New improved WriteToConsole().
  - 24th September 2006
* Minor tweaks - 28th September 2006
* Fixed a defect as per input by xircon (codeproject.com) and
  Visual C++ 2005 migration updates - 3rd Feb 2007
* Cut down CPU usage by updating WSAWaitForMultipleEvents()
  - 31st March 2007
==========================================================================
*/



#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>
#include <vector>

#include "ServerIOCP.h"

int main(int argc, char *argv[])
{
    //Validate the input
    if (argc < 2) {
        printf("\nUsage: %s port.", argv[0]);
        return 1;
    }

    if (false == Initialize()) {
        return 1;
    }

    SOCKET ListenSocket;
    struct sockaddr_in ServerAddress;
    //Overlapped I/O follows the model established in Windows and can be performed only on
    //sockets created through the WSASocket function
    ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    if (INVALID_SOCKET == ListenSocket) {
        printf("\nError occurred while opening socket: %d.", WSAGetLastError());
        goto error;
    } else {
        printf("\nWSASocket() successful.");
    }

    //Cleanup and Init with 0 the ServerAddress
    ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
    //Port number will be supplied as a command line argument
    int nPortNo;
    nPortNo = atoi(argv[1]);
    //Fill up the address structure
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = INADDR_ANY; //WinSock will supply address
    ServerAddress.sin_port = htons(nPortNo);    //comes from commandline

    //Assign local address and port number
    if (SOCKET_ERROR == bind(ListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) {
        closesocket(ListenSocket);
        printf("\nError occurred while binding.");
        goto error;
    } else {
        printf("\nbind() successful.");
    }

    //Make the socket a listening socket
    if (SOCKET_ERROR == listen(ListenSocket, SOMAXCONN)) {
        closesocket(ListenSocket);
        printf("\nError occurred while listening.");
        goto error;
    } else {
        printf("\nlisten() successful.");
    }

    g_hAcceptEvent = WSACreateEvent();

    if (WSA_INVALID_EVENT == g_hAcceptEvent) {
        printf("\nError occurred while WSACreateEvent().");
        goto error;
    }

    if (SOCKET_ERROR == WSAEventSelect(ListenSocket, g_hAcceptEvent, FD_ACCEPT)) {
        printf("\nError occurred while WSAEventSelect().");
        WSACloseEvent(g_hAcceptEvent);
        goto error;
    }

    printf("\nTo exit this server, hit a key at any time on this console...");
    DWORD nThreadID;
    g_hAcceptThread = CreateThread(0, 0, AcceptThread, (void *)ListenSocket, 0, &nThreadID);

    //Hang in there till a key is hit
    while (!_kbhit()) {
        Sleep(0);  //switch to some other thread
    }

    WriteToConsole("\nServer is shutting down...");
    //Start cleanup
    CleanUp();
    //Close open sockets
    closesocket(ListenSocket);
    DeInitialize();
    return 0; //success
error:
    closesocket(ListenSocket);
    DeInitialize();
    return 1;
}

bool Initialize()
{
    //Find out number of processors and threads
    g_nThreads = WORKER_THREADS_PER_PROCESSOR * GetNoOfProcessors();
    printf("\nNumber of processors on host: %d", GetNoOfProcessors());
    printf("\nThe following number of worker threads will be created: %d", g_nThreads);
    //Allocate memory to store thread handless
    g_phWorkerThreads = new HANDLE[g_nThreads];
    //Initialize the Console Critical Section
    InitializeCriticalSection(&g_csConsole);
    //Initialize the Client List Critical Section
    InitializeCriticalSection(&g_csClientList);
    //Create shutdown event
    g_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    // Initialize Winsock
    WSADATA wsaData;
    int nResult;
    nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (NO_ERROR != nResult) {
        printf("\nError occurred while executing WSAStartup().");
        return false; //error
    } else {
        printf("\nWSAStartup() successful.");
    }

    if (false == InitializeIOCP()) {
        printf("\nError occurred while initializing IOCP");
        return false;
    } else {
        printf("\nIOCP initialization successful.");
    }

    return true;
}

//Function to Initialize IOCP
bool InitializeIOCP()
{
    //Create I/O completion port
    g_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    if (NULL == g_hIOCompletionPort) {
        printf("\nError occurred while creating IOCP: %d.", WSAGetLastError());
        return false;
    }

    DWORD nThreadID;

    //Create worker threads
    for (int ii = 0; ii < g_nThreads; ii++) {
        g_phWorkerThreads[ii] = CreateThread(0, 0, WorkerThread, (void *)(ii + 1), 0, &nThreadID);
    }

    return true;
}

void CleanUp()
{
    //Ask all threads to start shutting down
    SetEvent(g_hShutdownEvent);
    //Let Accept thread go down
    WaitForSingleObject(g_hAcceptThread, INFINITE);

    for (int i = 0; i < g_nThreads; i++) {
        //Help threads get out of blocking - GetQueuedCompletionStatus()
        PostQueuedCompletionStatus(g_hIOCompletionPort, 0, (DWORD) NULL, NULL);
    }

    //Let Worker Threads shutdown
    WaitForMultipleObjects(g_nThreads, g_phWorkerThreads, TRUE, INFINITE);
    //We are done with this event
    WSACloseEvent(g_hAcceptEvent);
    //Cleanup dynamic memory allocations, if there are any.
    CleanClientList();
}

void DeInitialize()
{
    //Delete the Console Critical Section.
    DeleteCriticalSection(&g_csConsole);
    //Delete the Client List Critical Section.
    DeleteCriticalSection(&g_csClientList);
    //Cleanup IOCP.
    CloseHandle(g_hIOCompletionPort);
    //Clean up the event.
    CloseHandle(g_hShutdownEvent);
    //Clean up memory allocated for the storage of thread handles
    delete[] g_phWorkerThreads;
    //Cleanup Winsock
    WSACleanup();
}

//This thread will look for accept event
DWORD WINAPI AcceptThread(LPVOID lParam)
{
    SOCKET ListenSocket = (SOCKET)lParam;
    WSANETWORKEVENTS WSAEvents;

    //Accept thread will be around to look for accept event, until a Shutdown event is not Signaled.
    while (WAIT_OBJECT_0 != WaitForSingleObject(g_hShutdownEvent, 0)) {
        if (WSA_WAIT_TIMEOUT != WSAWaitForMultipleEvents(1, &g_hAcceptEvent, FALSE, WAIT_TIMEOUT_INTERVAL, FALSE)) {
            WSAEnumNetworkEvents(ListenSocket, g_hAcceptEvent, &WSAEvents);

            if ((WSAEvents.lNetworkEvents & FD_ACCEPT) && (0 == WSAEvents.iErrorCode[FD_ACCEPT_BIT])) {
                //Process it
                AcceptConnection(ListenSocket);
            }
        }
    }

    return 0;
}

//This function will process the accept event
void AcceptConnection(SOCKET ListenSocket)
{
    sockaddr_in ClientAddress;
    int nClientLength = sizeof(ClientAddress);
    //Accept remote connection attempt from the client
    SOCKET Socket = accept(ListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

    if (INVALID_SOCKET == Socket) {
        WriteToConsole("\nError occurred while accepting socket: %ld.", WSAGetLastError());
    }

    //Display Client's IP
    WriteToConsole("\nClient connected from: %s", inet_ntoa(ClientAddress.sin_addr));
    //Create a new ClientContext for this newly accepted client
    CClientContext   *pClientContext  = new CClientContext;
    pClientContext->SetOpCode(OP_READ);
    pClientContext->SetSocket(Socket);
    //Store this object
    AddToClientList(pClientContext);

    if (true == AssociateWithIOCP(pClientContext)) {
        //Once the data is successfully received, we will print it.
        pClientContext->SetOpCode(OP_WRITE);
        WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
        OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();
        //Get data.
        DWORD dwFlags = 0;
        DWORD dwBytes = 0;
        //Post initial Recv
        //This is a right place to post a initial Recv
        //Posting a initial Recv in WorkerThread will create scalability issues.
        int nBytesRecv = WSARecv(pClientContext->GetSocket(), p_wbuf, 1,
                                 &dwBytes, &dwFlags, p_ol, NULL);

        if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError())) {
            WriteToConsole("\nError in Initial Post.");
        }
    }
}

bool AssociateWithIOCP(CClientContext   *pClientContext)
{
    //Associate the socket with IOCP
    HANDLE hTemp = CreateIoCompletionPort((HANDLE)pClientContext->GetSocket(), g_hIOCompletionPort, (DWORD)pClientContext, 0);

    if (NULL == hTemp) {
        WriteToConsole("\nError occurred while executing CreateIoCompletionPort().");
        //Let's not work with this client
        RemoveFromClientListAndFreeMemory(pClientContext);
        return false;
    }

    return true;
}

//Worker thread will service IOCP requests
DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    int nThreadNo = (int)lpParam;
    void *lpContext = NULL;
    OVERLAPPED       *pOverlapped = NULL;
    CClientContext   *pClientContext = NULL;
    DWORD            dwBytesTransfered = 0;
    int nBytesRecv = 0;
    int nBytesSent = 0;
    DWORD             dwBytes = 0, dwFlags = 0;

    //Worker thread will be around to process requests, until a Shutdown event is not Signaled.
    while (WAIT_OBJECT_0 != WaitForSingleObject(g_hShutdownEvent, 0)) {
        BOOL bReturn = GetQueuedCompletionStatus(
                           g_hIOCompletionPort,
                           &dwBytesTransfered,
                           (LPDWORD)&lpContext,
                           &pOverlapped,
                           INFINITE);

        if (NULL == lpContext) {
            //We are shutting down
            break;
        }

        //Get the client context
        pClientContext = (CClientContext *)lpContext;

        if ((FALSE == bReturn) || ((TRUE == bReturn) && (0 == dwBytesTransfered))) {
            //Client connection gone, remove it.
            RemoveFromClientListAndFreeMemory(pClientContext);
            continue;
        }

        WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
        OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();

        switch (pClientContext->GetOpCode()) {
        case OP_READ:
            pClientContext->IncrSentBytes(dwBytesTransfered);

            //Write operation was finished, see if all the data was sent.
            //Else post another write.
            if (pClientContext->GetSentBytes() < pClientContext->GetTotalBytes()) {
                pClientContext->SetOpCode(OP_READ);
                p_wbuf->buf += pClientContext->GetSentBytes();
                p_wbuf->len = pClientContext->GetTotalBytes() - pClientContext->GetSentBytes();
                dwFlags = 0;
                //Overlapped send
                nBytesSent = WSASend(pClientContext->GetSocket(), p_wbuf, 1,
                                     &dwBytes, dwFlags, p_ol, NULL);

                if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError())) {
                    //Let's not work with this client
                    RemoveFromClientListAndFreeMemory(pClientContext);
                }
            } else {
                //Once the data is successfully received, we will print it.
                pClientContext->SetOpCode(OP_WRITE);
                pClientContext->ResetWSABUF();
                dwFlags = 0;
                //Get the data.
                nBytesRecv = WSARecv(pClientContext->GetSocket(), p_wbuf, 1,
                                     &dwBytes, &dwFlags, p_ol, NULL);

                if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError())) {
                    WriteToConsole("\nThread %d: Error occurred while executing WSARecv().", nThreadNo);
                    //Let's not work with this client
                    RemoveFromClientListAndFreeMemory(pClientContext);
                }
            }

            break;

        case OP_WRITE:
            char szBuffer[MAX_BUFFER_LEN];
            //Display the message we recevied
            pClientContext->GetBuffer(szBuffer);
            WriteToConsole("\nThread %d: The following message was received: %s", nThreadNo, szBuffer);
            //Send the message back to the client.
            pClientContext->SetOpCode(OP_READ);
            pClientContext->SetTotalBytes(dwBytesTransfered);
            pClientContext->SetSentBytes(0);
            p_wbuf->len  = dwBytesTransfered;
            dwFlags = 0;
            //Overlapped send
            nBytesSent = WSASend(pClientContext->GetSocket(), p_wbuf, 1,
                                 &dwBytes, dwFlags, p_ol, NULL);

            if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError())) {
                WriteToConsole("\nThread %d: Error occurred while executing WSASend().", nThreadNo);
                //Let's not work with this client
                RemoveFromClientListAndFreeMemory(pClientContext);
            }

            break;

        default:
            //We should never be reaching here, under normal circumstances.
            break;
        } // switch
    } // while

    return 0;
}

//Function to synchronize console output
//Threads need to be synchronized while they write to console.
//WriteConsole() API can be used, it is thread-safe, I think.
//I have created my own function.
void WriteToConsole(char *szFormat, ...)
{
    EnterCriticalSection(&g_csConsole);
    va_list args;
    va_start(args, szFormat);
    vprintf(szFormat, args);
    va_end(args);
    LeaveCriticalSection(&g_csConsole);
}

//Store client related information in a vector
void AddToClientList(CClientContext   *pClientContext)
{
    EnterCriticalSection(&g_csClientList);
    //Store these structures in vectors
    g_ClientContext.push_back(pClientContext);
    LeaveCriticalSection(&g_csClientList);
}

//This function will allow to remove one single client out of the list
void RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext)
{
    EnterCriticalSection(&g_csClientList);
    std::vector <CClientContext *>::iterator IterClientContext;

    //Remove the supplied ClientContext from the list and release the memory
    for (IterClientContext = g_ClientContext.begin(); IterClientContext != g_ClientContext.end(); IterClientContext++) {
        if (pClientContext == *IterClientContext) {
            g_ClientContext.erase(IterClientContext);
            //i/o will be cancelled and socket will be closed by destructor.
            delete pClientContext;
            break;
        }
    }

    LeaveCriticalSection(&g_csClientList);
}

//Clean up the list, this function will be executed at the time of shutdown
void CleanClientList()
{
    EnterCriticalSection(&g_csClientList);
    std::vector <CClientContext *>::iterator IterClientContext;

    for (IterClientContext = g_ClientContext.begin(); IterClientContext != g_ClientContext.end(); IterClientContext++) {
        //i/o will be cancelled and socket will be closed by destructor.
        delete *IterClientContext;
    }

    g_ClientContext.clear();
    LeaveCriticalSection(&g_csClientList);
}

//The use of static variable will ensure that
//we will make a call to GetSystemInfo()
//to find out number of processors,
//only if we don't have the information already.
//Repeated use of this function will be efficient.
int GetNoOfProcessors()
{
    static int nProcessors = 0;

    if (0 == nProcessors) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        nProcessors = si.dwNumberOfProcessors;
    }

    return nProcessors;
}