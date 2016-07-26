/***
*abort.c - abort a program by raising SIGABRT
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines abort() - print a message and raise SIGABRT.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <internal.h>
#include <rterr.h>
#include <signal.h>
#include <oscalls.h>
#include <mtdll.h>

#ifdef _DEBUG
#define _INIT_ABORT_BEHAVIOR _WRITE_ABORT_MSG
#else  /* _DEBUG */
#define _INIT_ABORT_BEHAVIOR (_WRITE_ABORT_MSG | _CALL_REPORTFAULT)
#endif  /* _DEBUG */

unsigned int __abort_behavior = _INIT_ABORT_BEHAVIOR;

/***
*void abort() - abort the current program by raising SIGABRT
*
*Purpose:
*   print out an abort message and raise the SIGABRT signal.  If the user
*   hasn't defined an abort handler routine, terminate the program
*   with exit status of 3 without cleaning up.
*
*   Multi-thread version does not raise SIGABRT -- this isn't supported
*   under multi-thread.
*
*Entry:
*   None.
*
*Exit:
*   Does not return.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl abort (
        void
        )
{
    _PHNDLR sigabrt_act = SIG_DFL;

    if (__abort_behavior & _WRITE_ABORT_MSG)
    {
        /* write the abort message */
        _NMSG_WRITE(_RT_ABORT);
    }


    /* Check if the user installed a handler for SIGABRT.
     * We need to read the user handler atomically in the case
     * another thread is aborting while we change the signal
     * handler.
     */
    sigabrt_act = __get_sigabrt();
    if (sigabrt_act != SIG_DFL)
    {
        raise(SIGABRT);
    }

    /* If there is no user handler for SIGABRT or if the user
     * handler returns, then exit from the program anyway
     */

    if (__abort_behavior & _CALL_REPORTFAULT)
    {
        /* Fake an exception to call reportfault. */
        EXCEPTION_RECORD ExceptionRecord;
        CONTEXT ContextRecord;
        EXCEPTION_POINTERS ExceptionPointers;

#ifdef _X86_
        __asm {
            mov dword ptr [ContextRecord.Eax], eax
            mov dword ptr [ContextRecord.Ecx], ecx
            mov dword ptr [ContextRecord.Edx], edx
            mov dword ptr [ContextRecord.Ebx], ebx
            mov dword ptr [ContextRecord.Esi], esi
            mov dword ptr [ContextRecord.Edi], edi
            mov word ptr [ContextRecord.SegSs], ss
            mov word ptr [ContextRecord.SegCs], cs
            mov word ptr [ContextRecord.SegDs], ds
            mov word ptr [ContextRecord.SegEs], es
            mov word ptr [ContextRecord.SegFs], fs
            mov word ptr [ContextRecord.SegGs], gs
            pushfd
            pop [ContextRecord.EFlags]
        }

        ContextRecord.ContextFlags = CONTEXT_CONTROL;
#pragma warning(push)
#pragma warning(disable:4311)
        ContextRecord.Eip = (ULONG)_ReturnAddress();
        ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
#pragma warning(pop)
        ContextRecord.Ebp = *((ULONG *)_AddressOfReturnAddress()-1);
#elif defined (_IA64_) || defined (_AMD64_)
        /* Need to fill up the Context in IA64 and AMD64. See VSW#286097 */
        RtlCaptureContext(&ContextRecord);
#else  /* defined (_IA64_) || defined (_AMD64_) */
        ZeroMemory(&ContextRecord, sizeof(ContextRecord));
#endif  /* defined (_IA64_) || defined (_AMD64_) */

        ZeroMemory(&ExceptionRecord, sizeof(ExceptionRecord));

        ExceptionRecord.ExceptionCode = STATUS_FATAL_APP_EXIT;
        ExceptionRecord.ExceptionAddress = _ReturnAddress();

        ExceptionPointers.ExceptionRecord = &ExceptionRecord;
        ExceptionPointers.ContextRecord = &ContextRecord;

        /* Make sure any filter already in place is deleted. */
        SetUnhandledExceptionFilter(NULL);

        UnhandledExceptionFilter(&ExceptionPointers);
    }


    /* If we don't want to call ReportFault, then we call _exit(3), which is the
     * same as invoking the default handler for SIGABRT
     */


    _exit(3);
}

/***
*unsigned int _set_abort_behavior(unsigned int, unsigned int) - set the behavior on abort
*
*Purpose:
*
*Entry:
*   unsigned int flags - the flags we want to set
*   unsigned int mask - mask the flag values
*
*Exit:
*   Return the old behavior flags
*
*Exceptions:
*   None
*
*******************************************************************************/

unsigned int __cdecl _set_abort_behavior(unsigned int flags, unsigned int mask)
{
    unsigned int oldflags = __abort_behavior;
    __abort_behavior = oldflags & (~mask) | flags & mask;
    return oldflags;
}
