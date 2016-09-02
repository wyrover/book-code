#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <winsock2.h>

int main(int argc, char* argv[])
{
    //Validate the input
    if (argc < 3) {
        printf("\nUsage: %s hostname port.", argv[0]);
        return 1; //error
    }

    // Initialize Winsock
    WSADATA wsaData;
    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (NO_ERROR != nResult) {
        printf("\nError occurred while executing WSAStartup().");
        return 1; //error
    } else {
        printf("\nWSAStartup() successful.");
    }

    SOCKET Socket;
    int nPortNo;
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    char szBuffer[256];
    //Port number will be supplied as a commandline argument
    nPortNo = atoi(argv[2]);
    //Create a socket
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == Socket) {
        printf("\nError occurred while opening socket: %ld.", WSAGetLastError());
        goto error;
    } else {
        printf("\nsocket() successful.");
    }

    //Server name will be supplied as a commandline argument
    //Get the server details
    Server = gethostbyname(argv[1]);

    if (Server == NULL) {
        closesocket(Socket);
        printf("\nError occurred no such host.");
        goto error;
    } else {
        printf("\ngethostbyname() successful.");
    }

    //Cleanup and Init with 0 the ServerAddress
    ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    //Assign the information received from gethostbyname()
    CopyMemory((char *)&ServerAddress.sin_addr.s_addr,
               (char *)Server->h_addr,
               Server->h_length);
    ServerAddress.sin_port = htons(nPortNo); //comes from commandline

    //Establish connection with the server
    if (SOCKET_ERROR == connect(Socket, reinterpret_cast<const struct sockaddr *>(&ServerAddress), sizeof(ServerAddress))) {
        closesocket(Socket);
        printf("\nError occurred while connecting.");
        goto error;
    } else {
        printf("\nconnect() successful.");
    }

    //Cleanup and Init with 0 the szBuffer
    ZeroMemory(szBuffer, 256);
    printf("\nPlease enter message to be sent to server: ");
    //Read the message from server
    fgets(szBuffer, 255, stdin);
    int nBytesSent;
    int nBytesRecv;
    //Send the message to the server
    nBytesSent = send(Socket, szBuffer, strlen(szBuffer), 0);

    if (SOCKET_ERROR == nBytesSent) {
        closesocket(Socket);
        printf("\nError occurred while writing to socket.");
        goto error;
    } else {
        printf("\nsend() successful.");
    }

    //Cleanup and Init with 0 the szBuffer
    ZeroMemory(szBuffer, 256);
    //Get the message from the server
    nBytesRecv = recv(Socket, szBuffer, 255, 0);

    if (SOCKET_ERROR == nBytesRecv) {
        closesocket(Socket);
        printf("\nError occurred while reading from socket.");
        goto error;
    } else {
        printf("\nrecv() successful.");
    }

    //Display the server message
    printf("\n%s", szBuffer);
    //Close the socket
    closesocket(Socket);
    //Cleanup Winsock
    WSACleanup();
    return 0; //success
error:
    // Cleanup Winsock
    WSACleanup();
    return 1; //error
}