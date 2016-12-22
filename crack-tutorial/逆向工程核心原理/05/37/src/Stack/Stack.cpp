#include "stdio.h"
#include "windows.h"

void main()
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    
    hFile = CreateFileA("c:\\work\\ReverseCore.txt",    // 1st - (string)
                        GENERIC_READ,                   // 2nd - 0x80000000
                        FILE_SHARE_READ,                // 3rd - 0x00000001
                        NULL,                           // 4th - 0000000000
                        OPEN_EXISTING,                  // 5th - 0x00000003
                        FILE_ATTRIBUTE_NORMAL,          // 6th - 0x00000080
                        NULL);                          // 7th - 0x00000000

    if( hFile != INVALID_HANDLE_VALUE )
        CloseHandle(hFile);
}