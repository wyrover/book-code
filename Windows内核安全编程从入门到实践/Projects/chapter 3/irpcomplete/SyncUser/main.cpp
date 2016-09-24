#include "windows.h"
#include "stdio.h"

int main()
{
    HANDLE hFile;
    char Buf[10] = {0};
    DWORD dwRet = 0;
    BOOL bRet;
    hFile = CreateFileA("\\\\.\\Sync0", GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        return 0;
    }

    bRet = ReadFile(hFile, Buf, 10, &dwRet, NULL);

    if (!bRet) {
        CloseHandle(hFile);
        return 0;
    }

    printf("SyncUser¡ª¡ªRead Buf: %s Read Len: %d\n", Buf, dwRet);
    return 0;
}