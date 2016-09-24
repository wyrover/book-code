#include "windows.h"
#include "stdio.h"

int main()
{
    HANDLE hFile;
    char Buf[3][10] = {0};
    DWORD dwRet[3] = {0};
    OVERLAPPED ol[3] = {0};
    HANDLE hEvent[3] = {0};
    hFile = CreateFileA("\\\\.\\Async0", GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        return 0;
    }

    hEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
    ol[0].hEvent = hEvent[0];
    hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
    ol[1].hEvent = hEvent[1];
    hEvent[2] = CreateEvent(NULL, TRUE, FALSE, NULL);
    ol[2].hEvent = hEvent[2];
    ReadFile(hFile, Buf[0], 10, &dwRet[0], &ol[0]);
    ReadFile(hFile, Buf[1], 10, &dwRet[1], &ol[1]);
    ReadFile(hFile, Buf[2], 10, &dwRet[2], &ol[2]);
    //do some other work here.
    WaitForMultipleObjects(3, hEvent, TRUE, INFINITE);
    printf("AsyncUser¡ª¡ªRead Buf1: %s\n\
		   Read Buf1: %s\n\
		   Read Buf1: %s\n",
           Buf[0], Buf[1], Buf[2]
          );
    CloseHandle(hFile);
    return 0;
}