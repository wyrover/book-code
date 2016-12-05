#include "stdio.h"
#include "windows.h"
#include "tchar.h"

LPVOID g_pOrgFilter = 0;

LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExcept)
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)g_pOrgFilter);

    // 8900    MOV DWORD PTR DS:[EAX], EAX
    // FFE0    JMP EAX
    pExcept->ContextRecord->Eip += 4;

    return EXCEPTION_CONTINUE_EXECUTION;
}

void AD_SetUnhandledExceptionFilter()
{
    printf("SEH : SetUnhandledExceptionFilter()\n");

    g_pOrgFilter = (LPVOID)SetUnhandledExceptionFilter(
                                (LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);

    __asm {
        xor eax, eax;
        mov dword ptr [eax], eax
        jmp eax                     // 확인 사살! 
    }
    
    printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    AD_SetUnhandledExceptionFilter();

    return 0;
}
