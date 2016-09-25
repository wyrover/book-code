/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS Run-Time Library
 * PURPOSE:           User-mode exception support for IA-32
 * FILE:              lib/rtl/i386/exception.c
 * PROGRAMERS:        Alex Ionescu (alex@relsoft.net)
 *                    Casper S. Hornstrup (chorns@users.sourceforge.net)
 */

/* INCLUDES *****************************************************************/
#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

//++ ree
RtlpExecuteHandler2();
RtlpGetExceptionList();
RtlpSetExceptionList();
RtlpExecuteHandlerForUnwind();
RtlpCaptureContext();
RtlpExecuteHandlerForException();

typedef struct _DISPATCHER_CONTEXT {
    PEXCEPTION_REGISTRATION_RECORD RegistrationPointer;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

//PUBLIC _RtlpGetStackLimits@8
//_RtlpGetStackLimits@8:
VOID
_declspec(naked)
NTAPI
RtlpGetStackLimits(PULONG_PTR LowLimit,
                   PULONG_PTR HighLimit)
{
    _asm{
        /* Get the stack limits */
        mov eax, fs:
        [TEB_STACK_LIMIT]//NtCurrentTeb()->NtTib.StackLimit;
        mov ecx, fs:
        [TEB_STACK_BASE]//NtCurrentTeb()->NtTib.StackBase;

        /* Return them */
        mov edx, [esp + 4]
        mov [edx], eax
        mov edx, [esp + 8]
        mov [edx], ecx

        /* return */
        ret 8
    }
}

NTSTATUS
NTAPI
ZwRaiseException(IN PEXCEPTION_RECORD ExceptionRecord,
                 IN PCONTEXT Context,
                 IN BOOLEAN SearchFrames)
{
    UNIMPLEMENTED
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
ZwContinue(IN PCONTEXT Context,
           IN BOOLEAN TestAlert)
{
    UNIMPLEMENTED
    return STATUS_SUCCESS;
}

//--

/* PUBLIC FUNCTIONS **********************************************************/

/*
 * @implemented
 */
//VOID
//NTAPI
//RtlGetCallersAddress(OUT PVOID *CallersAddress,
//                     OUT PVOID *CallersCaller)
//{
//    USHORT FrameCount;
//    PVOID  BackTrace[2];
//    PULONG BackTraceHash = NULL;
//
//    /* Get the tow back trace address */
//    FrameCount = RtlCaptureStackBackTrace(2, 2, &BackTrace[0],BackTraceHash);
//
//    /* Only if user want it */
//    if (*CallersAddress != NULL)
//    {
//        /* only when first frames exist */
//        if (FrameCount >= 1)
//        {
//            *CallersAddress = BackTrace[0];
//        }
//        else
//        {
//            *CallersAddress = NULL;
//        }
//    }
//
//    /* Only if user want it */
//    if (*CallersCaller != NULL)
//    {
//        /* only when second frames exist */
//        if (FrameCount >= 2)
//        {
//            *CallersCaller = BackTrace[1];
//        }
//        else
//        {
//            *CallersCaller = NULL;
//        }
//    }
//}

/*
 * @implemented
 */
BOOLEAN
NTAPI
RtlDispatchException(IN PEXCEPTION_RECORD ExceptionRecord,
                     IN PCONTEXT Context)
{
    PEXCEPTION_REGISTRATION_RECORD RegistrationFrame, NestedFrame = NULL;
    DISPATCHER_CONTEXT DispatcherContext;
    EXCEPTION_RECORD ExceptionRecord2;
    EXCEPTION_DISPOSITION Disposition;
    ULONG_PTR StackLow, StackHigh;
    ULONG_PTR RegistrationFrameEnd;
    /* Perform vectored exception handling (a dummy in kernel mode) */
    //if (RtlCallVectoredExceptionHandlers(ExceptionRecord, Context))
    //{
    //    /* Exception handled, continue execution */
    //    return TRUE;
    //}
    /* Get the current stack limits and registration frame */
    RtlpGetStackLimits(&StackLow, &StackHigh);
    RegistrationFrame = RtlpGetExceptionList();

    /* Now loop every frame */
    while (RegistrationFrame != EXCEPTION_CHAIN_END) {
        /* Registration chain entries are never NULL */
        ASSERT(RegistrationFrame != NULL);
        /* Find out where it ends */
        RegistrationFrameEnd = (ULONG_PTR)RegistrationFrame +
                               sizeof(EXCEPTION_REGISTRATION_RECORD);

        /* Make sure the registration frame is located within the stack */
        if ((RegistrationFrameEnd > StackHigh) ||
            ((ULONG_PTR)RegistrationFrame < StackLow) ||
            ((ULONG_PTR)RegistrationFrame & 0x3)) {
            /* Check if this happened in the DPC Stack */
            //if (RtlpHandleDpcStackException(RegistrationFrame,
            //                                RegistrationFrameEnd,
            //                                &StackLow,
            //                                &StackHigh))
            //{
            //    /* Use DPC Stack Limits and restart */
            //    continue;
            //}
            /* Set invalid stack and return false */
            ExceptionRecord->ExceptionFlags |= EXCEPTION_STACK_INVALID;
            return FALSE;
        }

        /* Check if logging is enabled */
        /*RtlpCheckLogException(ExceptionRecord,
                              Context,
                              RegistrationFrame,
                              sizeof(*RegistrationFrame));*/
        /* Call the handler */
        Disposition = RtlpExecuteHandlerForException(ExceptionRecord,
                      RegistrationFrame,
                      Context,
                      &DispatcherContext,
                      RegistrationFrame->Handler);

        /* Check if this is a nested frame */
        if (RegistrationFrame == NestedFrame) {
            /* Mask out the flag and the nested frame */
            ExceptionRecord->ExceptionFlags &= ~EXCEPTION_NESTED_CALL;
            NestedFrame = NULL;
        }

        /* Continue execution */
        if (Disposition == ExceptionContinueExecution) {
            /* Check if it was non-continuable */
            if (ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE) {
                /* Set up the exception record */
                ExceptionRecord2.ExceptionRecord = ExceptionRecord;
                ExceptionRecord2.ExceptionCode =
                    STATUS_NONCONTINUABLE_EXCEPTION;
                ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
                ExceptionRecord2.NumberParameters = 0;
                /* Raise the exception */
                RtlRaiseException(&ExceptionRecord2);
            } else {
                /* Return to caller */
                return TRUE;
            }
        }
        /* Continue searching */
        else if (Disposition == ExceptionContinueSearch) {
        }
        /* Nested exception */
        else if (Disposition == ExceptionNestedException) {
            /* Turn the nested flag on */
            ExceptionRecord->ExceptionFlags |= EXCEPTION_NESTED_CALL;

            /* Update the current nested frame */
            if (DispatcherContext.RegistrationPointer > NestedFrame) {
                /* Get the frame from the dispatcher context */
                NestedFrame = DispatcherContext.RegistrationPointer;
            }
        }
        /* Anything else */
        else {
            /* Set up the exception record */
            ExceptionRecord2.ExceptionRecord = ExceptionRecord;
            ExceptionRecord2.ExceptionCode = STATUS_INVALID_DISPOSITION;
            ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
            ExceptionRecord2.NumberParameters = 0;
            /* Raise the exception */
            RtlRaiseException(&ExceptionRecord2);
        }

        /* Handle the dispositions */
        //switch (Disposition)
        //{
        //    /* Continue searching */
        //    case ExceptionContinueExecution:
        //        /* Check if it was non-continuable */
        //        if (ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE)
        //        {
        //            /* Set up the exception record */
        //            ExceptionRecord2.ExceptionRecord = ExceptionRecord;
        //            ExceptionRecord2.ExceptionCode =
        //                STATUS_NONCONTINUABLE_EXCEPTION;
        //            ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
        //            ExceptionRecord2.NumberParameters = 0;
        //            /* Raise the exception */
        //            RtlRaiseException(&ExceptionRecord2);
        //        }
        //        else
        //        {
        //            /* Return to caller */
        //            return TRUE;
        //        }
        //    /* Continue searching */
        //    case ExceptionContinueSearch:
        //        break;
        //    /* Nested exception */
        //    case ExceptionNestedException:
        //        /* Turn the nested flag on */
        //        ExceptionRecord->ExceptionFlags |= EXCEPTION_NESTED_CALL;
        //        /* Update the current nested frame */
        //        if (DispatcherContext.RegistrationPointer > NestedFrame)
        //        {
        //            /* Get the frame from the dispatcher context */
        //            NestedFrame = DispatcherContext.RegistrationPointer;
        //        }
        //        break;
        //    /* Anything else */
        //    default:
        //        /* Set up the exception record */
        //        ExceptionRecord2.ExceptionRecord = ExceptionRecord;
        //        ExceptionRecord2.ExceptionCode = STATUS_INVALID_DISPOSITION;
        //        ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
        //        ExceptionRecord2.NumberParameters = 0;
        //        /* Raise the exception */
        //        RtlRaiseException(&ExceptionRecord2);
        //        break;
        //}
        /* Go to the next frame */
        RegistrationFrame = RegistrationFrame->Next;
    }

    /* Unhandled, return false */
    return FALSE;
}

/*
 * @implemented
 */
VOID
NTAPI
RtlUnwind(IN PVOID TargetFrame OPTIONAL,
          IN PVOID TargetIp OPTIONAL,
          IN PEXCEPTION_RECORD ExceptionRecord OPTIONAL,
          IN PVOID ReturnValue)
{
    PEXCEPTION_REGISTRATION_RECORD RegistrationFrame, OldFrame;
    DISPATCHER_CONTEXT DispatcherContext;
    EXCEPTION_RECORD ExceptionRecord2, ExceptionRecord3;
    EXCEPTION_DISPOSITION Disposition;
    ULONG_PTR StackLow, StackHigh;
    ULONG_PTR RegistrationFrameEnd;
    CONTEXT LocalContext;
    PCONTEXT Context;
    /* Get the current stack limits */
    RtlpGetStackLimits(&StackLow, &StackHigh);

    /* Check if we don't have an exception record */
    if (!ExceptionRecord) {
        /* Overwrite the argument */
        ExceptionRecord = &ExceptionRecord3;
        /* Setup a local one */
        ExceptionRecord3.ExceptionFlags = 0;
        ExceptionRecord3.ExceptionCode = STATUS_UNWIND;
        ExceptionRecord3.ExceptionRecord = NULL;
        ExceptionRecord3.ExceptionAddress = _ReturnAddress();
        ExceptionRecord3.NumberParameters = 0;
    }

    /* Check if we have a frame */
    if (TargetFrame) {
        /* Set it as unwinding */
        ExceptionRecord->ExceptionFlags |= EXCEPTION_UNWINDING;
    } else {
        /* Set the Exit Unwind flag as well */
        ExceptionRecord->ExceptionFlags |= (EXCEPTION_UNWINDING |
                                            EXCEPTION_EXIT_UNWIND);
    }

    /* Now capture the context */
    Context = &LocalContext;
    LocalContext.ContextFlags = CONTEXT_INTEGER |
                                CONTEXT_CONTROL |
                                CONTEXT_SEGMENTS;
    RtlpCaptureContext(Context);
    /* Pop the current arguments off */
    Context->Esp += sizeof(TargetFrame) +
                    sizeof(TargetIp) +
                    sizeof(ExceptionRecord) +
                    sizeof(ReturnValue);
    /* Set the new value for EAX */
    Context->Eax = (ULONG)ReturnValue;
    /* Get the current frame */
    RegistrationFrame = RtlpGetExceptionList();

    /* Now loop every frame */
    while (RegistrationFrame != EXCEPTION_CHAIN_END) {
        /* Registration chain entries are never NULL */
        ASSERT(RegistrationFrame != NULL);

        /* If this is the target */
        //if (RegistrationFrame == TargetFrame) ZwContinue(Context, FALSE);//ree
        if (RegistrationFrame == TargetFrame) {
            ZwContinue(Context, FALSE);
            return;
        }

        /* Check if the frame is too low */
        if ((TargetFrame) &&
            ((ULONG_PTR)TargetFrame < (ULONG_PTR)RegistrationFrame)) {
            /* Create an invalid unwind exception */
            ExceptionRecord2.ExceptionCode = STATUS_INVALID_UNWIND_TARGET;
            ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
            ExceptionRecord2.ExceptionRecord = ExceptionRecord;
            ExceptionRecord2.NumberParameters = 0;
            /* Raise the exception */
            RtlRaiseException(&ExceptionRecord2);
        }

        /* Find out where it ends */
        RegistrationFrameEnd = (ULONG_PTR)RegistrationFrame +
                               sizeof(EXCEPTION_REGISTRATION_RECORD);

        /* Make sure the registration frame is located within the stack */
        if ((RegistrationFrameEnd > StackHigh) ||
            ((ULONG_PTR)RegistrationFrame < StackLow) ||
            ((ULONG_PTR)RegistrationFrame & 0x3)) {
            /* Check if this happened in the DPC Stack */
            //if (RtlpHandleDpcStackException(RegistrationFrame,
            //                                RegistrationFrameEnd,
            //                                &StackLow,
            //                                &StackHigh))
            //{
            //    /* Use DPC Stack Limits and restart */
            //    continue;
            //}
            /* Create an invalid stack exception */
            ExceptionRecord2.ExceptionCode = STATUS_BAD_STACK;
            ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
            ExceptionRecord2.ExceptionRecord = ExceptionRecord;
            ExceptionRecord2.NumberParameters = 0;
            /* Raise the exception */
            RtlRaiseException(&ExceptionRecord2);
        } else {
            /* Call the handler */
            Disposition = RtlpExecuteHandlerForUnwind(ExceptionRecord,
                          RegistrationFrame,
                          Context,
                          &DispatcherContext,
                          RegistrationFrame->Handler);

            /* Continue searching */
            if (Disposition == ExceptionContinueSearch) {
                /* Continue searching */
            } else if (Disposition == ExceptionCollidedUnwind) {
                /* Get the original frame */
                RegistrationFrame = DispatcherContext.RegistrationPointer;
            } else {
                /* Set up the exception record */
                ExceptionRecord2.ExceptionRecord = ExceptionRecord;
                ExceptionRecord2.ExceptionCode = STATUS_INVALID_DISPOSITION;
                ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
                ExceptionRecord2.NumberParameters = 0;
                /* Raise the exception */
                RtlRaiseException(&ExceptionRecord2);
                break;
            }

            //switch(Disposition)
            //{
            //    /* Continue searching */
            //    case ExceptionContinueSearch:
            //        break;
            //    /* Collission */
            //    case ExceptionCollidedUnwind :
            //        /* Get the original frame */
            //        RegistrationFrame = DispatcherContext.RegistrationPointer;
            //        break;
            //    /* Anything else */
            //    default:
            //        /* Set up the exception record */
            //        ExceptionRecord2.ExceptionRecord = ExceptionRecord;
            //        ExceptionRecord2.ExceptionCode = STATUS_INVALID_DISPOSITION;
            //        ExceptionRecord2.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
            //        ExceptionRecord2.NumberParameters = 0;
            //        /* Raise the exception */
            //        RtlRaiseException(&ExceptionRecord2);
            //        break;
            //}
            /* Go to the next frame */
            OldFrame = RegistrationFrame;
            RegistrationFrame = RegistrationFrame->Next;
            /* Remove this handler */
            RtlpSetExceptionList(OldFrame);
        }
    }

    /* Check if we reached the end */
    if (TargetFrame == EXCEPTION_CHAIN_END) {
        /* Unwind completed, so we don't exit */
        ZwContinue(Context, FALSE);
    } else {
        /* This is an exit_unwind or the frame wasn't present in the list */
        ZwRaiseException(ExceptionRecord, Context, FALSE);
    }
}

/* EOF */