/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/ke/i386/exp.c
 * PURPOSE:         Exception Dispatching and Context<->Trap Frame Conversion
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 *                  Gregor Anich
 *                  Skywing (skywing@valhallalegends.com)
 */

/* INCLUDES ******************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>
#include "kdapi.h"

//++ ree
void
_declspec(naked)
NTAPI
KiSaveProcessorControlState(IN PKPROCESSOR_STATE ProcessorState)
{
    _asm{

        mov     edx, [esp + 4] // ProcessorState

        xor     ecx, ecx
#ifdef SIMULATE_DEBUG /* 用户态不能直接访问Crx寄存器 */
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr0], 0x80000011
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr2], 0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr3], 0x39000
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr4], 0

#else
        mov     eax, cr0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr0], eax
        mov     eax, cr2
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr2], eax
        mov     eax, cr3
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr3], eax
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr4], ecx
#endif
        test    KeFeatureBits, KF_CR4
        jz      short FeatureBits
        /* mov eax, cr4 编译器不支持 */
        _emit 0x0F
        _emit 0x20
        _emit 0xE0

        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.Cr4], eax
        FeatureBits:
#ifdef SIMULATE_DEBUG /* 为了模似调试环境，保存伪值，它被远程调试器使用 */
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr0], 0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr1], 0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr2], 0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr3], 0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr6], 0xffff0ff0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr7], 0x400

        mov     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Gdtr.Limit], 0x3ff
        mov     dword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Gdtr.Base], 0x20940

        mov     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Idtr.Limit], 0x7ff
        mov     dword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Idtr.Base], 0x20d40

        mov     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Tr], 0x28
        mov     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Ldtr], 0
#else
        mov     eax, dr0
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr0], eax
        mov     eax, dr1
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr1], eax
        mov     eax, dr2
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr2], eax
        mov     eax, dr3
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr3], eax
        mov     eax, dr6
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr6], eax

        mov     eax, dr7
        mov     dr7, ecx /* 设为0，这样调试器可以使用 */
        mov     [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr7], eax

        /* 保存 GDT & IDT 描述符表 */
        sgdt    fword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Gdtr.Limit]
        sidt    fword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Idtr.Limit]

        str     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Tr]
        sldt    word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Ldtr]
#endif
        retn    4
    }
}


void
_declspec(naked)
NTAPI
KiRestoreProcessorControlState(IN PKPROCESSOR_STATE ProcessorState)
{
    _asm{
        mov     edx, [esp + 4] //ProcessorState

#ifndef SIMULATE_DEBUG
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.Cr0]
        mov     cr0, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.Cr2]
        mov     cr2, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.Cr3]
        mov     cr3, eax
#endif
        test    KeFeatureBits, KF_CR4
        jz      short BITS
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.Cr4]
        //mov     cr4, eax
        _emit 0x0F
        _emit 0x22
        _emit 0xE0
        BITS:
#ifndef SIMULATE_DEBUG
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr0]
        mov     dr0, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr1]
        mov     dr1, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr2]
        mov     dr2, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr3]
        mov     dr3, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr6]
        mov     dr6, eax
        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.KernelDr7]
        mov     dr7, eax

        lgdt    fword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Gdtr.Limit]
        lidt    fword ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Idtr.Limit]

        mov     eax, [edx + KPROCESSOR_STATE.SpecialRegisters.Gdtr.Base]
        xor     ecx, ecx
        mov     cx,  word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Tr]
        add     eax, 5
        add     eax, ecx
        and     byte ptr [eax], NOT 2
        ltr     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Tr]

        lldt     word ptr [edx + KPROCESSOR_STATE.SpecialRegisters.Ldtr]
#endif
        retn    4
    }
}
//--

/* FUNCTIONS *****************************************************************/

ULONG
NTAPI
KiEspFromTrapFrame(IN PKTRAP_FRAME TrapFrame)
{
    /* Check if this is user-mode or V86 */
    if ((TrapFrame->SegCs & MODE_MASK) ||
        (TrapFrame->EFlags & EFLAGS_V86_MASK)) {
        /* Return it directly */
        return TrapFrame->HardwareEsp;
    } else {
        /* Edited frame */
        if (!(TrapFrame->SegCs & FRAME_EDITED)) {
            /* Return edited value */
            return TrapFrame->TempEsp;
        } else {
            /* Virgin frame, calculate */
            return (ULONG)&TrapFrame->HardwareEsp;
        }
    }
}

VOID
NTAPI
KiEspToTrapFrame(IN PKTRAP_FRAME TrapFrame,
                 IN ULONG Esp)
{
    //KIRQL OldIrql;
    ULONG Previous;
    /* Raise to APC_LEVEL if needed */
    //OldIrql = KeGetCurrentIrql();
    //if (OldIrql < APC_LEVEL) KeRaiseIrql(APC_LEVEL, &OldIrql);
    /* Get the old ESP */
    Previous = KiEspFromTrapFrame(TrapFrame);

    /* Check if this is user-mode or V86 */
    if ((TrapFrame->SegCs & MODE_MASK) ||
        (TrapFrame->EFlags & EFLAGS_V86_MASK)) {
        /* Write it directly */
        TrapFrame->HardwareEsp = Esp;
    } else {
        /* Don't allow ESP to be lowered, this is illegal */
        if (Esp < Previous) KeBugCheckEx(SET_OF_INVALID_CONTEXT,
                                             Esp,
                                             Previous,
                                             (ULONG_PTR)TrapFrame,
                                             0);

        /* Create an edit frame, check if it was alrady */
        if (!(TrapFrame->SegCs & FRAME_EDITED)) {
            /* Update the value */
            TrapFrame->TempEsp = Esp;
        } else {
            /* Check if ESP changed */
            if (Previous != Esp) {
                /* Save CS */
                TrapFrame->TempSegCs = TrapFrame->SegCs;
                TrapFrame->SegCs &= ~FRAME_EDITED;
                /* Save ESP */
                TrapFrame->TempEsp = Esp;
            }
        }
    }

    /* Restore IRQL */
    //if (OldIrql < APC_LEVEL) KeLowerIrql(OldIrql);
}

VOID
NTAPI
KiSsToTrapFrame(IN PKTRAP_FRAME TrapFrame,
                IN ULONG Ss)
{
    /* Remove the high-bits */
    Ss &= 0xFFFF;

    /* If this was V86 Mode */
    if (TrapFrame->EFlags & EFLAGS_V86_MASK) {
        /* Just write it */
        TrapFrame->HardwareSegSs = Ss;
    } else if (TrapFrame->SegCs & MODE_MASK) {
        /* Usermode, save the User SS */
        TrapFrame->HardwareSegSs = Ss | RPL_MASK;
    }
}

VOID
NTAPI
KeContextToTrapFrame(IN PCONTEXT Context,
                     IN OUT PKEXCEPTION_FRAME ExceptionFrame,
                     IN OUT PKTRAP_FRAME TrapFrame,
                     IN ULONG ContextFlags,
                     IN KPROCESSOR_MODE PreviousMode)
{
    /* Start with the basic Registers */
    if ((ContextFlags & CONTEXT_CONTROL) == CONTEXT_CONTROL) {
        /* Copy EFLAGS and sanitize them*/
        TrapFrame->EFlags = Context->EFlags;
        /* Copy EBP and EIP */
        TrapFrame->Ebp = Context->Ebp;
        TrapFrame->Eip = Context->Eip;
        /* Simply copy the CS value */
        TrapFrame->SegCs = Context->SegCs;
        /* Handle SS Specially for validation */
        TrapFrame->TempSegCs = Context->SegSs;
        /* Write ESP back; take into account Edited Trap Frames */
        TrapFrame->TempEsp = Context->Esp;
    }

    /* Process the Integer Registers */
    if ((ContextFlags & CONTEXT_INTEGER) == CONTEXT_INTEGER) {
        /* Copy them manually */
        TrapFrame->Eax = Context->Eax;
        TrapFrame->Ebx = Context->Ebx;
        TrapFrame->Ecx = Context->Ecx;
        TrapFrame->Edx = Context->Edx;
        TrapFrame->Esi = Context->Esi;
        TrapFrame->Edi = Context->Edi;
    }

    /* Process the Context Segments */
    if ((ContextFlags & CONTEXT_SEGMENTS) == CONTEXT_SEGMENTS) {
        /* For user mode, return the values directly */
        TrapFrame->SegDs = Context->SegDs;
        TrapFrame->SegEs = Context->SegEs;
        TrapFrame->SegFs = Context->SegFs;

        /* Handle GS specially */
        if (TrapFrame->SegCs == (KGDT_R3_CODE | RPL_MASK)) {
            /* Don't use it, if user */
            TrapFrame->SegGs = 0;
        } else {
            /* Copy it if kernel */
            TrapFrame->SegGs = Context->SegGs;
        }
    }

    /* Handle the Debug Registers */
    if ((ContextFlags & CONTEXT_DEBUG_REGISTERS) == CONTEXT_DEBUG_REGISTERS) {
        /* Copy Dr0 - Dr4 */
        TrapFrame->Dr0 = Context->Dr0;
        TrapFrame->Dr1 = Context->Dr1;
        TrapFrame->Dr2 = Context->Dr2;
        TrapFrame->Dr3 = Context->Dr3;
        /* Now sanitize and save DR6 */
        TrapFrame->Dr6 = Context->Dr6 & DR6_LEGAL;
        /* Sanitize and save DR7 */
        TrapFrame->Dr7 = Context->Dr7 & DR7_LEGAL;
    }
}

VOID
NTAPI
KeTrapFrameToContext(IN PKTRAP_FRAME TrapFrame,
                     IN PKEXCEPTION_FRAME ExceptionFrame,
                     IN OUT PCONTEXT Context)
{
    /* Start with the Control flags */
    if ((Context->ContextFlags & CONTEXT_CONTROL) == CONTEXT_CONTROL) {
        /* EBP, EIP and EFLAGS */
        Context->Ebp = TrapFrame->Ebp;
        Context->Eip = TrapFrame->Eip;
        Context->EFlags = TrapFrame->EFlags;

        /* Return the correct CS */
        if (!(TrapFrame->SegCs & FRAME_EDITED) &&
            !(TrapFrame->EFlags & EFLAGS_V86_MASK)) {
            /* Get it from the Temp location */
            Context->SegCs = TrapFrame->TempSegCs & 0xFFFF;
        } else {
            /* Return it directly */
            Context->SegCs = TrapFrame->SegCs & 0xFFFF;
        }

        /* Get the Ss and ESP */
        Context->SegSs = TrapFrame->TempSegCs;
        Context->Esp = TrapFrame->TempEsp;
    }

    /* Handle the Segments */
    if ((Context->ContextFlags & CONTEXT_SEGMENTS) == CONTEXT_SEGMENTS) {
        /* Return the segments */
        Context->SegGs = TrapFrame->SegGs & 0xFFFF;
        Context->SegFs = TrapFrame->SegFs & 0xFFFF;
        Context->SegEs = TrapFrame->SegEs & 0xFFFF;
        Context->SegDs = TrapFrame->SegDs & 0xFFFF;
    }

    /* Handle the simple registers */
    if ((Context->ContextFlags & CONTEXT_INTEGER) == CONTEXT_INTEGER) {
        /* Return them directly */
        Context->Eax = TrapFrame->Eax;
        Context->Ebx = TrapFrame->Ebx;
        Context->Ecx = TrapFrame->Ecx;
        Context->Edx = TrapFrame->Edx;
        Context->Esi = TrapFrame->Esi;
        Context->Edi = TrapFrame->Edi;
    }

    /* Handle debug registers */
    if ((Context->ContextFlags & CONTEXT_DEBUG_REGISTERS) ==
        CONTEXT_DEBUG_REGISTERS) {
        /* Make sure DR7 is valid */
        if (TrapFrame->Dr7 & ~DR7_RESERVED_MASK) {
            /* Copy the debug registers */
            Context->Dr0 = TrapFrame->Dr0;
            Context->Dr1 = TrapFrame->Dr1;
            Context->Dr2 = TrapFrame->Dr2;
            Context->Dr3 = TrapFrame->Dr3;
            Context->Dr6 = TrapFrame->Dr6;
            /* Update DR7 */
            Context->Dr7 = TrapFrame->Dr7;
        } else {
            /* Otherwise clear DR registers */
            Context->Dr0 =
                Context->Dr1 =
                    Context->Dr2 =
                        Context->Dr3 =
                            Context->Dr6 =
                                Context->Dr7 = 0;
        }
    }
}


VOID
NTAPI
KiDispatchException(IN PEXCEPTION_RECORD ExceptionRecord,
                    IN PKEXCEPTION_FRAME ExceptionFrame,
                    IN PKTRAP_FRAME TrapFrame,
                    IN KPROCESSOR_MODE PreviousMode,
                    IN BOOLEAN FirstChance)
{
    CONTEXT Context;
    EXCEPTION_RECORD LocalExceptRecord;
    /* Increase number of Exception Dispatches */
    KdpKeGetCurrentPrcb()->KeExceptionDispatchCount++;
    /* Set the context flags */
    Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    /* Get a Context */
    KeTrapFrameToContext(TrapFrame, ExceptionFrame, &Context);

    /* Look at our exception code */
    switch (ExceptionRecord->ExceptionCode) {
    /* Breakpoint */
    case STATUS_BREAKPOINT:
#ifdef SIMULATE_DEBUG
        Context.Eip -= 5;
#else
        /* Decrement EIP by one */
        Context.Eip--;
#endif
        break;

    /* Internal exception */
    case KI_EXCEPTION_ACCESS_VIOLATION:
        /* Set correct code */
        ExceptionRecord->ExceptionCode = STATUS_ACCESS_VIOLATION;

        if (PreviousMode == UserMode) {
            /* FIXME: Handle no execute */
        }

        break;
    }

    /* Sanity check */
    ASSERT(!((PreviousMode == KernelMode) &&
             (Context.EFlags & EFLAGS_V86_MASK)));

    /* Handle kernel-mode first, it's simpler */
    if (PreviousMode == KernelMode) {
        /* Check if this is a first-chance exception */
        if (FirstChance == TRUE) {
            /* Break into the debugger for the first time */
            if (KiDebugRoutine(TrapFrame,
                               ExceptionFrame,
                               ExceptionRecord,
                               &Context,
                               PreviousMode,
                               FALSE)) {
                /* Exception was handled */
                goto Handled;
            }

            /* If the Debugger couldn't handle it, dispatch the exception */
            if (RtlDispatchException(ExceptionRecord, &Context)) goto Handled;
        }

        /* This is a second-chance exception, only for the debugger */
        if (KiDebugRoutine(TrapFrame,
                           ExceptionFrame,
                           ExceptionRecord,
                           &Context,
                           PreviousMode,
                           TRUE)) {
            /* Exception was handled */
            goto Handled;
        }

        /* Third strike; you're out */
        KeBugCheckEx(KMODE_EXCEPTION_NOT_HANDLED,
                     ExceptionRecord->ExceptionCode,
                     (ULONG_PTR)ExceptionRecord->ExceptionAddress,
                     (ULONG_PTR)TrapFrame,
                     0);
    } else {
        /* User mode exception, was it first-chance? */
        if (FirstChance) {
            /*
             * Break into the kernel debugger unless a user mode debugger
             * is present or user mode exceptions are ignored, except if this
             * is a debug service which we must always pass to KD
             */
            if ((!(KdpPsGetCurrentProcess()->DebugPort) &&
                 !(KdIgnoreUmExceptions)) ||
                (KdIsThisAKdTrap(ExceptionRecord,
                                 &Context,
                                 PreviousMode))) {
                /* Call the kernel debugger */
                if (KiDebugRoutine(TrapFrame,
                                   ExceptionFrame,
                                   ExceptionRecord,
                                   &Context,
                                   PreviousMode,
                                   FALSE)) {
                    /* Exception was handled */
                    goto Handled;
                }
            }

            /* Forward exception to user mode debugger */
            if (DbgkForwardException(ExceptionRecord, TRUE, FALSE)) return;

            /* Set up the user-stack */
DispatchToUser:
            //_SEH2_TRY
            {
                ULONG Size;
                ULONG_PTR Stack, NewStack;

                /* Make sure we have a valid SS and that this isn't V86 mode */
                if ((TrapFrame->HardwareSegSs != (KGDT_R3_DATA | RPL_MASK)) ||
                    (TrapFrame->EFlags & EFLAGS_V86_MASK)) {
                    /* Raise an exception instead */
                    LocalExceptRecord.ExceptionCode = STATUS_ACCESS_VIOLATION;
                    LocalExceptRecord.ExceptionFlags = 0;
                    LocalExceptRecord.NumberParameters = 0;
                    RtlRaiseException(&LocalExceptRecord);
                }

                /* Align context size and get stack pointer */
                Size = (sizeof(CONTEXT) + 3) & ~3;
                Stack = (Context.Esp & ~3) - Size;
                /* Probe stack and copy Context */
                ProbeForWrite((PVOID)Stack, Size, sizeof(ULONG));
                RtlCopyMemory((PVOID)Stack, &Context, sizeof(CONTEXT));
                /* Align exception record size and get stack pointer */
                Size = (sizeof(EXCEPTION_RECORD) -
                        (EXCEPTION_MAXIMUM_PARAMETERS -
                         ExceptionRecord->NumberParameters) *
                        sizeof(ULONG) + 3) & ~3;
                NewStack = Stack - Size;
                /* Probe stack and copy exception record */
                ProbeForWrite((PVOID)(NewStack - 2 * sizeof(ULONG_PTR)),
                              Size +  2 * sizeof(ULONG_PTR),
                              sizeof(ULONG));
                RtlCopyMemory((PVOID)NewStack, ExceptionRecord, Size);
                /* Now write the two params for the user-mode dispatcher */
                *(PULONG_PTR)(NewStack - 1 * sizeof(ULONG_PTR)) = Stack;
                *(PULONG_PTR)(NewStack - 2 * sizeof(ULONG_PTR)) = NewStack;
                /* Set new Stack Pointer */
                KiSsToTrapFrame(TrapFrame, KGDT_R3_DATA);
                KiEspToTrapFrame(TrapFrame, NewStack - 2 * sizeof(ULONG_PTR));
                /* Force correct segments */
                TrapFrame->SegCs = Ke386SanitizeSeg(KGDT_R3_CODE, PreviousMode);
                TrapFrame->SegDs = Ke386SanitizeSeg(KGDT_R3_DATA, PreviousMode);
                TrapFrame->SegEs = Ke386SanitizeSeg(KGDT_R3_DATA, PreviousMode);
                TrapFrame->SegFs = Ke386SanitizeSeg(KGDT_R3_TEB,  PreviousMode);
                TrapFrame->SegGs = 0;
                /* Set EIP to the User-mode Dispatcher */
                TrapFrame->Eip = (ULONG)KeUserExceptionDispatcher;
                /* Dispatch exception to user-mode */
                _SEH2_YIELD(return);
            }
            /*_SEH2_EXCEPT((RtlCopyMemory(&LocalExceptRecord,
                _SEH2_GetExceptionInformation()->ExceptionRecord,
                sizeof(EXCEPTION_RECORD)), EXCEPTION_EXECUTE_HANDLER))*/
            {
                /* Check if we got a stack overflow and raise that instead */
                if ((NTSTATUS)LocalExceptRecord.ExceptionCode ==
                    STATUS_STACK_OVERFLOW) {
                    /* Copy the exception address and record */
                    LocalExceptRecord.ExceptionAddress =
                        ExceptionRecord->ExceptionAddress;
                    RtlCopyMemory(ExceptionRecord,
                                  (PVOID)&LocalExceptRecord,
                                  sizeof(EXCEPTION_RECORD));
                    /* Do the exception again */
                    _SEH2_YIELD(goto DispatchToUser);
                }
            }
            //_SEH2_END;
        }

        /* Try second chance */
        if (DbgkForwardException(ExceptionRecord, TRUE, TRUE)) {
            /* Handled, get out */
            return;
        } else if (DbgkForwardException(ExceptionRecord, FALSE, TRUE)) {
            /* Handled, get out */
            return;
        }

        /* 3rd strike, kill the process */
        DPRINT1("Kill %.16s, ExceptionCode: %lx, ExceptionAddress: %lx, BaseAddress: %lx\n",
                PsGetCurrentProcess()->ImageFileName,
                ExceptionRecord->ExceptionCode,
                ExceptionRecord->ExceptionAddress,
                PsGetCurrentProcess()->SectionBaseAddress);
        ZwTerminateProcess(NtCurrentProcess(), ExceptionRecord->ExceptionCode);
        KeBugCheckEx(KMODE_EXCEPTION_NOT_HANDLED,
                     ExceptionRecord->ExceptionCode,
                     (ULONG_PTR)ExceptionRecord->ExceptionAddress,
                     (ULONG_PTR)TrapFrame,
                     0);
    }

Handled:
    /* Convert the context back into Trap/Exception Frames */
    KeContextToTrapFrame(&Context,
                         ExceptionFrame,
                         TrapFrame,
                         Context.ContextFlags,
                         PreviousMode);
    return;
}

VOID
NTAPI
KiDispatchException2(IN PEXCEPTION_RECORD ExceptionRecord,
                     IN PKEXCEPTION_FRAME ExceptionFrame,
                     IN PKTRAP_FRAME TrapFrame,
                     IN KPROCESSOR_MODE PreviousMode,
                     IN BOOLEAN FirstChance)
{
    CONTEXT Context;
    EXCEPTION_RECORD LocalExceptRecord;
    /* Increase number of Exception Dispatches */
    KdpKeGetCurrentPrcb()->KeExceptionDispatchCount++;
    /* Set the context flags */
    Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    /* Get a Context */
    KeTrapFrameToContext(TrapFrame, ExceptionFrame, &Context);

    /* Look at our exception code */
    switch (ExceptionRecord->ExceptionCode) {
    /* Breakpoint */
    case STATUS_BREAKPOINT:
#ifdef SIMULATE_DEBUG
        Context.Eip -= 5;
#else
        /* Decrement EIP by one */
        Context.Eip--;
#endif
        break;

    /* Internal exception */
    case KI_EXCEPTION_ACCESS_VIOLATION:
        /* Set correct code */
        ExceptionRecord->ExceptionCode = STATUS_ACCESS_VIOLATION;

        if (PreviousMode == UserMode) {
            /* FIXME: Handle no execute */
        }

        break;
    }

    /* Sanity check */
    ASSERT(!((PreviousMode == KernelMode) &&
             (Context.EFlags & EFLAGS_V86_MASK)));

    /* Handle kernel-mode first, it's simpler */
    if (PreviousMode == KernelMode) {
        /* Check if this is a first-chance exception */
        if (FirstChance == TRUE) {
            /* Break into the debugger for the first time */
            if (KiDebugRoutine(TrapFrame,
                               ExceptionFrame,
                               ExceptionRecord,
                               &Context,
                               PreviousMode,
                               FALSE)) {
                /* Exception was handled */
                goto Handled;
            }

            /* If the Debugger couldn't handle it, dispatch the exception */
            if (RtlDispatchException(ExceptionRecord, &Context)) goto Handled;
        }

        /* This is a second-chance exception, only for the debugger */
        if (KiDebugRoutine(TrapFrame,
                           ExceptionFrame,
                           ExceptionRecord,
                           &Context,
                           PreviousMode,
                           TRUE)) {
            /* Exception was handled */
            goto Handled;
        }

        /* Third strike; you're out */
        KeBugCheckEx(KMODE_EXCEPTION_NOT_HANDLED,
                     ExceptionRecord->ExceptionCode,
                     (ULONG_PTR)ExceptionRecord->ExceptionAddress,
                     (ULONG_PTR)TrapFrame,
                     0);
    } else {
        /* User mode exception, was it first-chance? */
        if (FirstChance) {
            /*
             * Break into the kernel debugger unless a user mode debugger
             * is present or user mode exceptions are ignored, except if this
             * is a debug service which we must always pass to KD
             */
            if ((!(KdpPsGetCurrentProcess()->DebugPort) &&
                 !(KdIgnoreUmExceptions)) ||
                (KdIsThisAKdTrap(ExceptionRecord,
                                 &Context,
                                 PreviousMode))) {
                /* Call the kernel debugger */
                if (KiDebugRoutine(TrapFrame,
                                   ExceptionFrame,
                                   ExceptionRecord,
                                   &Context,
                                   PreviousMode,
                                   FALSE)) {
                    /* Exception was handled */
                    goto Handled;
                }
            }

            /* Forward exception to user mode debugger */
            if (DbgkForwardException(ExceptionRecord, TRUE, FALSE)) return;

            /* Set up the user-stack */
DispatchToUser:
            //_SEH2_TRY
            {
                ULONG Size;
                ULONG_PTR Stack, NewStack;

                /* Make sure we have a valid SS and that this isn't V86 mode */
                if ((TrapFrame->HardwareSegSs != (KGDT_R3_DATA | RPL_MASK)) ||
                    (TrapFrame->EFlags & EFLAGS_V86_MASK)) {
                    /* Raise an exception instead */
                    LocalExceptRecord.ExceptionCode = STATUS_ACCESS_VIOLATION;
                    LocalExceptRecord.ExceptionFlags = 0;
                    LocalExceptRecord.NumberParameters = 0;
                    RtlRaiseException(&LocalExceptRecord);
                }

                /* Align context size and get stack pointer */
                Size = (sizeof(CONTEXT) + 3) & ~3;
                Stack = (Context.Esp & ~3) - Size;
                /* Probe stack and copy Context */
                ProbeForWrite((PVOID)Stack, Size, sizeof(ULONG));
                RtlCopyMemory((PVOID)Stack, &Context, sizeof(CONTEXT));
                /* Align exception record size and get stack pointer */
                Size = (sizeof(EXCEPTION_RECORD) -
                        (EXCEPTION_MAXIMUM_PARAMETERS -
                         ExceptionRecord->NumberParameters) *
                        sizeof(ULONG) + 3) & ~3;
                NewStack = Stack - Size;
                /* Probe stack and copy exception record */
                ProbeForWrite((PVOID)(NewStack - 2 * sizeof(ULONG_PTR)),
                              Size +  2 * sizeof(ULONG_PTR),
                              sizeof(ULONG));
                RtlCopyMemory((PVOID)NewStack, ExceptionRecord, Size);
                /* Now write the two params for the user-mode dispatcher */
                *(PULONG_PTR)(NewStack - 1 * sizeof(ULONG_PTR)) = Stack;
                *(PULONG_PTR)(NewStack - 2 * sizeof(ULONG_PTR)) = NewStack;
                /* Set new Stack Pointer */
                KiSsToTrapFrame(TrapFrame, KGDT_R3_DATA);
                KiEspToTrapFrame(TrapFrame, NewStack - 2 * sizeof(ULONG_PTR));
                /* Force correct segments */
                TrapFrame->SegCs = Ke386SanitizeSeg(KGDT_R3_CODE, PreviousMode);
                TrapFrame->SegDs = Ke386SanitizeSeg(KGDT_R3_DATA, PreviousMode);
                TrapFrame->SegEs = Ke386SanitizeSeg(KGDT_R3_DATA, PreviousMode);
                TrapFrame->SegFs = Ke386SanitizeSeg(KGDT_R3_TEB,  PreviousMode);
                TrapFrame->SegGs = 0;
                /* Set EIP to the User-mode Dispatcher */
                TrapFrame->Eip = (ULONG)KeUserExceptionDispatcher;
                /* Dispatch exception to user-mode */
                _SEH2_YIELD(return);
            }
            /*_SEH2_EXCEPT((RtlCopyMemory(&LocalExceptRecord,
                _SEH2_GetExceptionInformation()->ExceptionRecord,
                sizeof(EXCEPTION_RECORD)), EXCEPTION_EXECUTE_HANDLER))*/
            {
                /* Check if we got a stack overflow and raise that instead */
                if ((NTSTATUS)LocalExceptRecord.ExceptionCode ==
                    STATUS_STACK_OVERFLOW) {
                    /* Copy the exception address and record */
                    LocalExceptRecord.ExceptionAddress =
                        ExceptionRecord->ExceptionAddress;
                    RtlCopyMemory(ExceptionRecord,
                                  (PVOID)&LocalExceptRecord,
                                  sizeof(EXCEPTION_RECORD));
                    /* Do the exception again */
                    _SEH2_YIELD(goto DispatchToUser);
                }
            }
            //_SEH2_END;
        }

        /* Try second chance */
        if (DbgkForwardException(ExceptionRecord, TRUE, TRUE)) {
            /* Handled, get out */
            return;
        } else if (DbgkForwardException(ExceptionRecord, FALSE, TRUE)) {
            /* Handled, get out */
            return;
        }

        /* 3rd strike, kill the process */
        DPRINT1("Kill %.16s, ExceptionCode: %lx, ExceptionAddress: %lx, BaseAddress: %lx\n",
                PsGetCurrentProcess()->ImageFileName,
                ExceptionRecord->ExceptionCode,
                ExceptionRecord->ExceptionAddress,
                PsGetCurrentProcess()->SectionBaseAddress);
        ZwTerminateProcess(NtCurrentProcess(), ExceptionRecord->ExceptionCode);
        KeBugCheckEx(KMODE_EXCEPTION_NOT_HANDLED,
                     ExceptionRecord->ExceptionCode,
                     (ULONG_PTR)ExceptionRecord->ExceptionAddress,
                     (ULONG_PTR)TrapFrame,
                     0);
    }

Handled:
    /* Convert the context back into Trap/Exception Frames */
    KeContextToTrapFrame(&Context,
                         ExceptionFrame,
                         TrapFrame,
                         Context.ContextFlags,
                         PreviousMode);
    return;
}