#ifndef __SRV_H
#define __SRV_H

#include <windows.h>

class CServer
{
public:
    static BOOL GetTextFileContents(HANDLE hEvent, PWSTR pszFileName, PSTR pBuffer, DWORD dwBufferLen);
    
private:
    CServer(){};
    ~CServer(){};

    static DWORD WINAPI RequestWorker(LPVOID lpParameter);

    struct WorkerData
    {
        PWSTR pszFileName;
        PSTR pBuffer;
        DWORD dwBufferLen;
        HANDLE hCompletionHandle;
    } ;
} ;

#endif
