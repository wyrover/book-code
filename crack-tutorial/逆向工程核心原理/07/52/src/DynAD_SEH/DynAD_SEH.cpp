#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void AD_BreakPoint()
{
    printf("SEH : BreakPoint\n");

    __asm {
        // install SEH
        push handler
        push DWORD ptr fs:[0]
        mov DWORD ptr fs:[0], esp
        
        // generating exception
        int 3

        // 1) debugging
        //    go to terminating code
        mov eax, 0xFFFFFFFF
        jmp eax                 // process terminating!!!

        // 2) not debugging
        //    go to normal code
handler:
        mov eax, dword ptr ss:[esp+0xc]
        mov ebx, normal_code
        mov dword ptr ds:[eax+0xb8], ebx
        xor eax, eax
        retn

normal_code:
        //   remove SEH
        pop dword ptr fs:[0]
        add esp, 4
    }

    printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    AD_BreakPoint();

    return 0;
}
