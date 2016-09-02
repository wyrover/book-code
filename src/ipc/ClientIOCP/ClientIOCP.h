#ifndef _CLIENT_IOCP_H_
#define _CLIENT_IOCP_H_

//Disable deprecation warnings
#pragma warning(disable: 4996)

//Buffer Length
#define MAX_BUFFER_LEN 256

CRITICAL_SECTION g_csConsole; //When threads write to console we need mutual exclusion

//Structure to pass information to Thread
struct ThreadInfo {
    int m_nThreadNo;
    int m_nNoOfSends;
    SOCKET m_Socket;
    char m_szBuffer[MAX_BUFFER_LEN];
};

//global functions
bool CreateConnectedSocket(SOCKET *pSocket, char *szHost, int nPortNo);
void WriteToConsole(char *szFormat, ...);
DWORD WINAPI WorkerThread(LPVOID lpParam);

#endif