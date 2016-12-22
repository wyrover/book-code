#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void DynAD_INT2D()
{
    BOOL bDebugging = FALSE;

    __asm {
        // install SEH
        push handler
        push DWORD ptr fs:[0]
        mov DWORD ptr fs:[0], esp
        
        int 0x2d

        nop
        mov bDebugging, 1
        jmp normal_code

handler:
        mov eax, dword ptr ss:[esp+0xc]
        mov dword ptr ds:[eax+0xb8], offset normal_code
        mov bDebugging, 0
        xor eax, eax
        retn

normal_code:
        //   remove SEH
        pop dword ptr fs:[0]
        add esp, 4
    }

    printf("Trap Flag (INT 2D)\n");
    if( bDebugging )  printf("  => Debugging!!!\n\n");
    else              printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    DynAD_INT2D();

    return 0;
}
