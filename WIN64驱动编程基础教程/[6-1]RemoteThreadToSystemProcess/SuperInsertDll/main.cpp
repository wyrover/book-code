#include <stdio.h>
#include <Windows.h>
#include "main.h"

typedef long (__fastcall *RTLADJUSTPRIVILEGE64)(ULONG, ULONG, ULONG, PVOID);
RTLADJUSTPRIVILEGE64 RtlAdjustPrivilege;

int main()
{
    WCHAR dllname[MAX_PATH] = {0}; //L"c:\\testdll.DLL";
    DWORD dwPID = 0, dwRetVal = 0;
    RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE64)GetProcAddress(LoadLibraryW(L"ntdll.dll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege(20, 1, 0, &dwRetVal); //debug
    printf("input pid: ");
    scanf("%ld", &dwPID);
    printf("input dll full path: ");
    scanf("%ws", dllname);
    InjectDllExW(dwPID, dllname);
    getchar();
    printf("inject over!");
    getchar();
    return 0;
}