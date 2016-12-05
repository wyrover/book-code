#include "stdio.h"
#include "windows.h"
#include "tchar.h"

DWORD g_dwOrgChecksum = 0xF5934986;

int _tmain(int argc, TCHAR* argv[]);

void DynAD_Checksum()
{
    BOOL bDebugging = FALSE;
    DWORD dwSize = 0;
    printf("Checksum\n");
    
    __asm {
        mov ecx, offset _tmain
        mov esi, offset DynAD_Checksum
        sub ecx, esi            // ecx : loop count (buf size)
        xor eax, eax            // eax : checksum
        xor ebx, ebx

_CALC_CHECKSUM:
        movzx ebx, byte ptr ds:[esi]
        add eax, ebx
        rol eax, 1
        inc esi
        loop _CALC_CHECKSUM

        cmp eax, g_dwOrgChecksum
        je _NOT_DEBUGGING
        mov bDebugging, 1

_NOT_DEBUGGING:
    }

    if( bDebugging )  printf("  => Debugging!!!\n\n");
    else              printf("  => Not debugging...\n\n");
}


int _tmain(int argc, TCHAR* argv[])
{
    DynAD_Checksum();

    return 0;
}
