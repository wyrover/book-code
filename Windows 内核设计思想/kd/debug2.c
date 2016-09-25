/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS Run-Time Library
 * PURPOSE:           Debug Routines
 * FILE:              lib/rtl/i386/debug.S
 * PROGRAMER:         Alex Ionescu (alex@relsoft.net)
 */
#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

/* GLOBALS ****************************************************************/
_KiTrap03();
_KiDebugService();
/* FUNCTIONS **************************************************************/

VOID
NTAPI
DbgBreakPointNoBugCheck(VOID)
{
    _asm {
        int 3
    }
}

VOID
NTAPI
DbgBreakPoint(VOID)
{
    _asm {
#ifdef SIMULATE_DEBUG
        pushfd
        push cs
        call _KiTrap03
#endif
        int 3
    }
}

VOID
NTAPI
DbgBreakPointWithStatus(
    _In_ ULONG Status
)
{
    _asm {
        mov eax, Status
        int 3
    }
}

VOID
NTAPI
RtlpBreakWithStatusInstruction(VOID)
{
    _asm {
        int 3
    }
}

VOID
NTAPI
DebugService2(
    IN PVOID Argument1,
    IN PVOID Argument2,
    IN ULONG Service
)
{
    _asm {
        /* Call the interrupt */
        mov eax, Service
        mov ecx, Argument1
        mov edx, Argument2
#ifdef SIMULATE_DEBUG
        pushfd
        push cs
        call _KiDebugService
        int 3
#else
        int HEX(2D)
        int 3
#endif
    }
}

ULONG
NTAPI
DebugService(
    IN ULONG Service,
    IN PVOID Argument1,
    IN PVOID Argument2,
    IN PVOID Argument3,
    IN PVOID Argument4
)
{
    _asm {
        /* Call the Interrupt */
        mov eax, Service
        mov ecx, Argument1
        mov edx, Argument2
        mov ebx, Argument3
        mov edi, Argument4
#ifdef SIMULATE_DEBUG
        pushfd
        push cs
        call _KiDebugService
        int 3
        sub esp, 8
#else
        int HEX(2D)
        int 3
#endif
    }
}
