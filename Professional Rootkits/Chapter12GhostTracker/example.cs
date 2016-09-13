#include <stdio.h>
#include "winsock2.h"

void main() {

  //----------------------
  // Initialize Winsock.
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != NO_ERROR)
    printf("Error at WSAStartup()\n");

  //----------------------
  // Create a SOCKET for listening for
  // incoming connection requests.
  SOCKET ListenSocket;
  ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return;
  }

  //----------------------
  // The sockaddr_in structure specifies the address family,
  // IP address, and port for the socket that is being bound.
  sockaddr_in service;
  service.sin_family = AF_INET;
  service.sin_addr.s_addr = inet_addr("127.0.0.1");
  service.sin_port = htons(27015);

  if (bind( ListenSocket, 
    (SOCKADDR*) &service, 
    sizeof(service)) == SOCKET_ERROR) {
    printf("bind() failed.\n");
    closesocket(ListenSocket);
    return;
  }

  //----------------------
  // Listen for incoming connection requests.
  // on the created socket
  if (listen( ListenSocket, 1 ) == SOCKET_ERROR)
    printf("Error listening on socket.\n");

  //----------------------
  // Create a SOCKET for accepting incoming requests.
  SOCKET AcceptSocket;
  printf("Waiting for client to connect...\n");

  //----------------------
  // Accept the connection.
  while(1) {
    AcceptSocket = SOCKET_ERROR;
    while( AcceptSocket == SOCKET_ERROR ) {
      AcceptSocket = accept( ListenSocket, NULL, NULL );
    }
    printf("Client connected.\n");
    ListenSocket = AcceptSocket;
    break;
  }

  WSACleanup();
  return;
}
