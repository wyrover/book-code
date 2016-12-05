#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void PEB()
{
    HMODULE hMod = NULL;
    FARPROC pProc = NULL;
    LPBYTE pTEB = NULL;
    LPBYTE pPEB = NULL;
    BOOL bIsDebugging = FALSE;
    
    // IsDebuggerPresent()
    bIsDebugging = IsDebuggerPresent();
    printf("IsDebuggerPresent() = %d\n", bIsDebugging);
    if( bIsDebugging )  printf("  => Debugging!!!\n\n");
    else                printf("  => Not debugging...\n\n");

    // Ldr
    pProc = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCurrentTeb");
    pTEB = (LPBYTE)(*pProc)();               // address of TEB
    pPEB = (LPBYTE)*(LPDWORD)(pTEB+0x30);     // address of PEB

    printf("PEB.Ldr\n");
    DWORD pLdrSig[4] = { 0xEEFEEEFE, 0xEEFEEEFE, 0xEEFEEEFE, 0xEEFEEEFE };
    LPBYTE pLdr = (LPBYTE)*(LPDWORD)(pPEB+0xC);
    __try 
    {
        while( TRUE )
        {
            if( !memcmp(pLdr, pLdrSig, sizeof(pLdrSig)) )
            {
                printf("  => Debugging!!!\n\n");
                break;
            }

            pLdr++;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("  => Not debugging...\n\n");
    }

    // Process Heap - Flags
    bIsDebugging = FALSE;
    LPBYTE pHeap = (LPBYTE)*(LPDWORD)(pPEB+0x18);
    DWORD dwFlags = *(LPDWORD)(pHeap+0xC);
    printf("PEB.ProcessHeap.Flags = 0x%X\n", dwFlags);
    if( dwFlags != 0x2 )  printf("  => Debugging!!!\n\n");
    else                  printf("  => Not debugging...\n\n");

    // Process Heap - ForceFlags
    bIsDebugging = FALSE;
    DWORD dwForceFlags = *(LPDWORD)(pHeap+0x10);
    printf("PEB.ProcessHeap.ForceFlags = 0x%X\n", dwForceFlags);
    if( dwForceFlags != 0x0 )  printf("  => Debugging!!!\n\n");
    else                       printf("  => Not debugging...\n\n");

    // NtGlobalFlag
    bIsDebugging = FALSE;
    DWORD dwNtGlobalFlag = *(LPDWORD)(pPEB+0x68);
    printf("PEB.NtGlobalFlag = 0x%X\n", dwNtGlobalFlag);
    if( (dwNtGlobalFlag & 0x70) == 0x70 )  printf("  => Debugging!!!\n\n");
    else                                   printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    PEB();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}