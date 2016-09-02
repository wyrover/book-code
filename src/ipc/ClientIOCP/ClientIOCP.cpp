/*
==========================================================================

Purpose:

This is a sample code that demonstrates for the following:

* Use of the I/O Completion ports with WinSock.  The idea is to create a
  simple application that will use IOCP, highlight how to use IOCP.

Notes:

* The client will make simple socket calls.
  Let server use IOCP to manage them.

Author:

* Swarajya Pendharkar

Date:

* 10th March 2006

Updates:

* Minor tweaks - 24th March 2006
* Removed a comment - 9th June 2006
* Updates for stress testing, client will keep running till
  we want it to - 19th August 2006
* Client is now multi-threaded - 22th August 2006
* New improved WriteToConsole() - 24th September 2006
* Visual C++ 2005 migration updates - 3rd Feb 2007
==========================================================================
*/



#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>

#include "ClientIOCP.h"

int main(int argc, char* argv[])
{
    //Validate the input
    if (argc < 3) {
        printf("\nUsage: %s hostname port.", argv[0]);
        return 1; //error
    }

    //Initialize Winsock
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (NO_ERROR != nResult) {
        printf("\nError occurred while executing WSAStartup().");
        return 1; //error
    }

    //Initialize the Console Critical Section
    InitializeCriticalSection(&g_csConsole);
    int nPortNo = atoi(argv[2]);
    char szBuffer[MAX_BUFFER_LEN];
    int nNoOfThreads = 0;
    int nNoOfSends = 0;
    printf("\nPlease enter message to be sent to the server: ");
    //Read the message from server
    gets(szBuffer);
    printf("\nPlease enter number of threads to be created: ");
    //No. of times we will send the message to the server
    scanf("%d", &nNoOfThreads);
    printf("\nPlease enter number of times the messages needs to be sent: ");
    //No. of times we will send the message to the server
    scanf("%d", &nNoOfSends);
    HANDLE *p_hThreads = new HANDLE[nNoOfThreads];
    ThreadInfo *pThreadInfo = new ThreadInfo[nNoOfThreads];
    bool bConnectedSocketCreated = false;
    DWORD nThreadID;

    for (int ii = 0; ii < nNoOfThreads; ii++) {
        bConnectedSocketCreated = CreateConnectedSocket(&(pThreadInfo[ii].m_Socket), argv[1], nPortNo);

        if (!bConnectedSocketCreated) {
            //Clean up memory
            delete[] p_hThreads;
            delete[] pThreadInfo;
            //failed in creating of connected socket, error out.
            return 1;
        }

        //Populate ThreadInfo
        pThreadInfo[ii].m_nNoOfSends = nNoOfSends;
        pThreadInfo[ii].m_nThreadNo = ii + 1;
        sprintf(pThreadInfo[ii].m_szBuffer, "Thread %d - %s", ii + 1, szBuffer);
        //Create thread and start banging the server
        p_hThreads[ii] = CreateThread(0, 0, WorkerThread, (void *)(&pThreadInfo[ii]), 0, &nThreadID);
    }

    //Let Worker Threads shutdown
    WaitForMultipleObjects(nNoOfThreads, p_hThreads, TRUE, INFINITE);

    //Close the sockets here
    for (int ii = 0; ii < nNoOfThreads; ii++) {
        closesocket(pThreadInfo[ii].m_Socket);
    }

    //Clean up memory
    delete[] p_hThreads;
    delete[] pThreadInfo;
    //Delete the Console Critical Section.
    DeleteCriticalSection(&g_csConsole);
    //Cleanup Winsock
    WSACleanup();
    return 0;
}

//vprintf() is not thread safe
void WriteToConsole(char *szFormat, ...)
{
    EnterCriticalSection(&g_csConsole);
    va_list args;
    va_start(args, szFormat);
    vprintf(szFormat, args);
    va_end(args);
    LeaveCriticalSection(&g_csConsole);
}

bool CreateConnectedSocket(SOCKET *pSocket, char *szHost, int nPortNo)
{
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    //Create a socket
    *pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == *pSocket) {
        WriteToConsole("\nError occurred while opening socket: %d.", WSAGetLastError());
        return false; //error
    }

    //Server name will be supplied as a commandline argument
    //Get the server details
    Server = gethostbyname(szHost);

    if (Server == NULL) {
        closesocket(*pSocket);
        WriteToConsole("\nError occurred no such host.");
        return false; //error
    }

    //Cleanup and Init with 0 the ServerAddress
    ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    //Assign the information received from gethostbyname()
    CopyMemory((char *)&ServerAddress.sin_addr.s_addr,
               (char *)Server->h_addr,
               Server->h_length);
    ServerAddress.sin_port = htons(nPortNo);

    //Establish connection with the server
    if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress), sizeof(ServerAddress))) {
        closesocket(*pSocket);
        WriteToConsole("\nError occurred while connecting.");
        return false; //error
    }

    return true;
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    ThreadInfo *pThreadInfo = (ThreadInfo*)lpParam;
    char szTemp[MAX_BUFFER_LEN];
    int nBytesSent = 0;
    int nBytesRecv = 0;

    for (int ii = 0; ii < pThreadInfo->m_nNoOfSends; ii++) {
        sprintf(szTemp, "%d. %s", ii + 1, pThreadInfo->m_szBuffer);
        //Send the message to the server, include the NULL as well
        nBytesSent = send(pThreadInfo->m_Socket, szTemp, strlen(szTemp), 0);

        if (SOCKET_ERROR == nBytesSent) {
            WriteToConsole("\nError occurred while writing to socket %ld.", WSAGetLastError());
            return 1; //error
        }

        //Get the message from the server
        nBytesRecv = recv(pThreadInfo->m_Socket, szTemp, 255, 0);

        if (SOCKET_ERROR == nBytesRecv) {
            WriteToConsole("\nError occurred while reading from socket %ld.", WSAGetLastError());
            return 1; //error
        }

        //Display the server message
        WriteToConsole("\nServer sent the following message: %s", szTemp);
    }

    return 0; //success
}