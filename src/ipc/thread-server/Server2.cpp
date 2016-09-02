#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>

#define ACK_MESG_RECV "Message received successfully"

//global functions
void AcceptConnections(SOCKET ListenSocket);
DWORD WINAPI AcceptHandler(void* Socket);

int main(int argc, char *argv[])
{
    //Validate the input
    if (argc < 2) {
        printf("\nUsage: %s port.", argv[0]);
        goto error;
    }

    // Initialize Winsock
    WSADATA wsaData;
    int nResult;
    nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (NO_ERROR != nResult) {
        printf("\nError occurred while executing WSAStartup().");
        return 1; //error
    } else {
        printf("\nWSAStartup() successful.");
    }

    SOCKET ListenSocket;
    int    nPortNo;
    struct sockaddr_in ServerAddress;
    //Create a socket
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == ListenSocket) {
        printf("\nError occurred while opening socket: %ld.", WSAGetLastError());
        goto error;
    } else {
        printf("\nsocket() successful.");
    }

    //Cleanup and Init with 0 the ServerAddress
    ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
    //Port number will be supplied as a commandline argument
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

    //This function will take are of multiple clients using threads
    AcceptConnections(ListenSocket);
    //Close open sockets
    closesocket(ListenSocket);
    //Cleanup Winsock
    WSACleanup();
    return 0; //success
error:
    // Cleanup Winsock
    WSACleanup();
    return 1; //error
}

//This function will loop on while creating a new thread for each client connection
void AcceptConnections(SOCKET ListenSocket)
{
    sockaddr_in ClientAddress;
    int nClientLength = sizeof(ClientAddress);

    //Infinite, no graceful shutdown of server implemented,
    //preferably server should be implemented as a service
    //Events can also be used for graceful shutdown
    while (1) {
        //Accept remote connection attempt from the client
        SOCKET Socket = accept(ListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

        if (INVALID_SOCKET == Socket) {
            printf("\nError occurred while accepting socket: %ld.", WSAGetLastError());
        }

        //Display Client's IP
        printf("\nClient connected from: %s", inet_ntoa(ClientAddress.sin_addr));
        DWORD nThreadID;
        //Spawn one thread for each client connection, not a wise idea.
        //One should limit the number of threads or use I/O completion port
        CreateThread(0, 0, AcceptHandler, (void*)Socket, 0, &nThreadID);
    }
}

//Thread procedure one thread will be created for each client.
DWORD WINAPI AcceptHandler(void* Socket)
{
    SOCKET RemoteSocket = (SOCKET)Socket;
    char szBuffer[256];
    //Cleanup and Init with 0 the szBuffer
    ZeroMemory(szBuffer, 256);
    int nBytesSent;
    int nBytesRecv;
    //Receive data from a connected or bound socket
    nBytesRecv = recv(RemoteSocket, szBuffer, 255, 0);

    if (SOCKET_ERROR == nBytesRecv) {
        closesocket(RemoteSocket);
        printf("\nError occurred while receiving from socket.");
        return 1; //error
    } else {
        printf("\nrecv() successful.");
    }

    //Display the message received on console
    printf("\nThe following message was received: %s", szBuffer);
    //Send data on a connected socket to the client
    nBytesSent = send(RemoteSocket, ACK_MESG_RECV , strlen(ACK_MESG_RECV), 0);

    if (SOCKET_ERROR == nBytesSent) {
        closesocket(RemoteSocket);
        printf("\nError occurred while writing to socket.");
        return 1; //error
    } else {
        printf("\nsend() successful.");
    }

    return 0; //success
}