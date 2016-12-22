#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void DynAD_SingleStep()
{
    printf("Trap Flag (Single Step)\n");

    __asm {
        // install SEH
        push handler
        push DWORD ptr fs:[0]
        mov DWORD ptr fs:[0], esp
        
        pushfd
        or dword ptr ss:[esp], 0x100
        popfd
        nop

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
    DynAD_SingleStep();

    return 0;
}
