#include <windows.h>
#include <iostream>

using namespace std;

DWORD checksumPE(unsigned char* lpPE,DWORD szPE)
{
    unsigned short* lpcc = (unsigned short*)lpPE;
    DWORD checksum = 0;
    DWORD oft = ((PIMAGE_DOS_HEADER)lpPE)->e_lfanew;
    oft += offsetof(IMAGE_NT_HEADERS,OptionalHeader.CheckSum);
    for (DWORD i=0;i<szPE;i+=2)
    {
        if (i>=(oft-1) && i < (oft+4))
        {
            if ((oft % 2) != 0)
            {
                checksum+= *lpcc++ & 0xFF;
                lpcc++;
                checksum+= *lpcc++ & 0xFF00;
                i+=4;
            }else
            {
                lpcc+=2;
                i+=2;
            }
        }else
            checksum+=*lpcc++;
        if ((checksum | 0xFFFF) != 0xFFFF)
        {
            checksum++;
            checksum &= 0xFFFF;
        }
    }
    checksum += szPE;
    return checksum;
}

int main()
{
    HANDLE hf = CreateFileA("d:\\notepad.exe",
                            GENERIC_READ,FILE_SHARE_READ,
                            0,OPEN_EXISTING,0,0);
    HANDLE hMap = CreateFileMappingA(hf,0,PAGE_READONLY,0,0,0);
    DWORD szFile = GetFileSize(hf,0);
    void* lpFile = (void*)MapViewOfFile(hMap,FILE_MAP_READ,0,0,szFile);
    DWORD checksum1 = checksumPE((unsigned char*)lpFile,szFile);
    PIMAGE_NT_HEADERS imnh = (PIMAGE_NT_HEADERS)((const char*)lpFile +
                                                 ((PIMAGE_DOS_HEADER)lpFile)->e_lfanew);
    if (checksum1 != imnh->OptionalHeader.CheckSum)
        cout << "文件被篡改!" << endl;
    return 0;
}

