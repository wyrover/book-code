#include "stdio.h"
#include "windows.h"
#include "tchar.h"

void DynAD_RDTSC()
{
    DWORD dwDelta = 0;

    printf("Timing Check (RDTSC method)");

    __asm {
		pushad
		rdtsc
		push edx
		push eax
		xor eax, eax
		mov ecx, 0x3e8

_LOOP_START:
		inc eax
		loop _LOOP_START

		rdtsc
		pop esi			// eax
		pop edi			// edx
		// check high order bits
		cmp edx, edi
		ja _DEBUGGER_FOUND
		// check low order bits
		sub eax, esi
        mov dwDelta, eax
		cmp eax, 0xffffff
		jb _DEBUGGER_NOT_FOUND

        // debugger found -> crash!!!
_DEBUGGER_FOUND:
		xor eax, eax
		mov [eax], eax

        // debugger not found
_DEBUGGER_NOT_FOUND:
		popad
	}
    
    printf(" : delta = %X (ticks)\n", dwDelta);
    printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    DynAD_RDTSC();

    return 0;
}
