/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            ntoskrnl/ke/i386/ctxswitch.S
 * PURPOSE:         Thread Context Switching
 *
 * PROGRAMMERS:     Alex Ionescu (alex@relsoft.net)
 *                  Gregor Anich (FPU Code)
 */

/* INCLUDES ******************************************************************/
#include <ntoskrnl.h>
//#include <asm.inc>
//#include <ks386.inc>

//++ ree
typedef struct _KSWITCHFRAME {
    PVOID ExceptionList;
    BOOLEAN ApcBypassDisable;
    PVOID RetAddr;
} KSWITCHFRAME, *PKSWITCHFRAME;

VOID
FASTCALL
KiSwapContextEntry(IN PKSWITCHFRAME SwitchFrame,
                   IN ULONG_PTR OldThreadAndApcFlag);

BOOLEAN
FASTCALL
KiSwapContextExit(IN PKTHREAD OldThread,
                  IN PKSWITCHFRAME SwitchFrame);

VOID
FASTCALL
KiEnterV86Mode(IN PKV8086_STACK_FRAME StackFrame);
//--

//EXTERN @KiSwapContextEntry@8:PROC
//EXTERN @KiSwapContextExit@8:PROC
//EXTERN @KiRetireDpcList@4:PROC
//EXTERN @KiEnterV86Mode@4:PROC
//EXTERN @KiExitV86Mode@4:PROC

/* FUNCTIONS ****************************************************************/
//.code
_declspec(naked) KiSwapContextInternal()
{
    _asm{
//PUBLIC @KiSwapContextInternal@0
//@KiSwapContextInternal@0:
        /* Build switch frame */
        sub esp, 2 * 4
        mov ecx, esp
        jmp KiSwapContextEntry//@8
    }
}

BOOLEAN
_declspec(naked)
FASTCALL
KiSwapContext(
    IN KIRQL WaitIrql,
    IN PKTHREAD CurrentThread
)
{
    _asm{
//PUBLIC @KiSwapContext@8
//@KiSwapContext@8:
        /* Save 4 registers */
        sub esp, 4 * 4

        /* Save all the non-volatile ones */
        mov [esp + 12], ebx
        mov [esp + 8], esi
        mov [esp + 4], edi
        mov [esp + 0], ebp

        /* Get the wait IRQL */
        or dl, cl

        /* Do the swap with the registers correctly setup */
        call KiSwapContextInternal//@0

        /* Return the registers */
        mov ebp, [esp + 0]
        mov edi, [esp + 4]
        mov esi, [esp + 8]
        mov ebx, [esp + 12]

        /* Clean stack */
        add esp, 4 * 4
        ret
    }
}

VOID
_declspec(naked)
FASTCALL
KiSwitchThreads(
    IN PKTHREAD OldThread,
    IN PKTHREAD NewThread
)
{
    _asm{
//PUBLIC @KiSwitchThreads@8
//@KiSwitchThreads@8:
        /* Load the new kernel stack and switch OS to new thread */
        mov esp, edx
        call KiSwapContextExit//@8

        /* Now we're on the new thread. Return to the caller to restore registers */
        add esp, 2 * 4
        ret
    }
}

VOID
_declspec(naked)
FASTCALL
KiRetireDpcListInDpcStack(
    IN PKPRCB Prcb,
    IN PVOID DpcStack
)
{
    _asm{
//PUBLIC @KiRetireDpcListInDpcStack@8
//@KiRetireDpcListInDpcStack@8:
        /* Switch stacks and retire DPCs */
        mov eax, esp
        mov esp, edx
        push eax
        call KiRetireDpcList//@4

        /* Return on original stack */
        pop esp
        ret
    }
}

VOID
_declspec(naked)
NTAPI
Ki386SetupAndExitToV86Mode(
    OUT PTEB VdmTeb
)
{
    _asm{
        /* FIXFIX: Move to C code ****/
//PUBLIC _Ki386SetupAndExitToV86Mode@4
//_Ki386SetupAndExitToV86Mode@4:

        /* Enter V8086 mode */
        pushad
        sub esp, (12 + KTRAP_FRAME_LENGTH + NPX_FRAME_LENGTH)
        mov ecx, esp
        call KiEnterV86Mode//@4
        //jmp $
        _while:
        jmp _while
    }
}

VOID
_declspec(naked)
FASTCALL
Ki386BiosCallReturnAddress(
    IN PKTRAP_FRAME TrapFrame
)
{
    _asm{
//PUBLIC @Ki386BiosCallReturnAddress@4
//@Ki386BiosCallReturnAddress@4:

        /* Exit V8086 mode */
        call KiExitV86Mode//@4
        mov esp, eax
        add esp, (12 + KTRAP_FRAME_LENGTH + NPX_FRAME_LENGTH)
        popad
        ret 4

    }
}//END
