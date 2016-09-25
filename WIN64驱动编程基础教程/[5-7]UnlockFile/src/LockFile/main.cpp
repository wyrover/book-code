#include <stdio.h>
#include <Windows.h>

typedef long (*RTLADJUSTPRIVILEGE)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef long (*NTDUPLICATEOBJECT)(HANDLE, HANDLE, HANDLE, PHANDLE, ACCESS_MASK, BOOLEAN, ULONG);

void GetAllProcessA(int pids[], int *procount)
{
    int i = 0, c = 0;
    HANDLE hProcess = 0;

    for (i = 8; i < 19996; i += 4) {
        hProcess = OpenProcess(0x10, 0, i);

        if (hProcess != 0) {
            pids[c] = i;
            CloseHandle(hProcess);
            c++;
        }
    }

    *procount = c;
}

int main()
{
    BOOLEAN bRet;
    int pids[260];
    int procsnum = 0;
    char pFile[260];
    RTLADJUSTPRIVILEGE getdbg = (RTLADJUSTPRIVILEGE)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlAdjustPrivilege");
    NTDUPLICATEOBJECT NtDuplicateObject = (NTDUPLICATEOBJECT)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDuplicateObject");
    getdbg(20, 1, 0, &bRet);
    memset(pids, 0, 4 * 260);
    memset(pFile, 0, 260);
    printf("Input the file name you want to protect: ");
    scanf("%s", pFile);
    HANDLE hsFile = CreateFileA(pFile, 0x80000000, 0, 0, 3, 0, 0);
    SetHandleInformation(hsFile, 0, 2);
    GetAllProcessA(pids, &procsnum);

    for (int i = 0; i < procsnum; i++) {
        HANDLE htFile = 0;
        HANDLE hProcess = OpenProcess(0x1F0FFF, 0, pids[i]);

        if (hProcess != 0) {
            NtDuplicateObject((HANDLE) - 1, hsFile, hProcess, &htFile, 0, 0, 4);
            CloseHandle(hProcess);
        }
    }

    getchar();
    printf("OK!\n");
    getchar();
    return 0;
}