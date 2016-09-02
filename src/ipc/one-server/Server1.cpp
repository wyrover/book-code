#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>

#define ACK_MESG_RECV "Message received successfully"

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

    SOCKET ListenSocket, RemoteSocket;
    int    nPortNo, nClientLength;
    char szBuffer[256];
    struct sockaddr_in ServerAddress, ClientAddress;
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

    nClientLength = sizeof(ClientAddress);
    //Accept remote connection attempt from the client
    RemoteSocket = accept(ListenSocket, (struct sockaddr *) &ClientAddress, &nClientLength);

    if (INVALID_SOCKET == RemoteSocket) {
        closesocket(ListenSocket);
        printf("\nError occurred while accepting socket: %ld.", WSAGetLastError());
        goto error;
    } else {
        printf("\naccept() successful.");
    }

    //Display Client's IP
    printf("\nClient connected from: %s", inet_ntoa(ClientAddress.sin_addr));
    //Cleanup and Init with 0 the szBuffer
    ZeroMemory(szBuffer, 256);
    int nBytesSent;
    int nBytesRecv;
    //Receive data from a connected or bound socket
    nBytesRecv = recv(RemoteSocket, szBuffer, 255, 0);

    if (SOCKET_ERROR == nBytesRecv) {
        closesocket(ListenSocket);
        closesocket(RemoteSocket);
        printf("\nError occurred while receiving from socket.");
        goto error;
    } else {
        printf("\nrecv() successful.");
    }

    //Display the message received on console
    printf("\nThe following message was received: %s", szBuffer);
    //Send data on a connected socket to the client
    nBytesSent = send(RemoteSocket, ACK_MESG_RECV , strlen(ACK_MESG_RECV), 0);

    if (SOCKET_ERROR == nBytesSent) {
        closesocket(ListenSocket);
        closesocket(RemoteSocket);
        printf("\nError occurred while writing to socket.");
        goto error;
    } else {
        printf("\nsend() successful.");
    }

    //Close open sockets
    closesocket(ListenSocket);
    closesocket(RemoteSocket);
    //Cleanup Winsock
    WSACleanup();
    return 0; //success
error:
    // Cleanup Winsock
    WSACleanup();
    return 1; //error
}