#include "windows.h"
#include "stdio.h"

int main()
{
    HANDLE hFile;
    hFile = CreateFileA(
                "\\\\.\\Callback0",
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

    if (INVALID_HANDLE_VALUE == hFile) {
        printf("CreateFile Failure!err:%d\n",
               GetLastError());
        return 0;
    }

    CloseHandle(hFile);
    return 0;
}