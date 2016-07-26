/***
*gs_report.c - Report a /GS security check failure
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines function used to report a security check failure.
*       This file corresponds to gs_report.c in the Windows sources.
*
*       Entrypoints:
*            __report_gsfailure
*
*******************************************************************************/

#include <windows.h>

#if defined (_CRTBLD) && !defined (_SYSCRT)
#include <dbgint.h>     /* needed for _CRT_DEBUGGER_HOOK */
#endif  /* defined (_CRTBLD) && !defined (_SYSCRT) */

#if defined (_AMD64_)

PRUNTIME_FUNCTION
RtlLookupFunctionEntry (
    IN ULONG64 ControlPc,
    OUT PULONG64 ImageBase,
    IN OUT PVOID HistoryTable OPTIONAL
    );

PVOID
RtlVirtualUnwind (
    IN ULONG HandlerType,
    IN ULONG64 ImageBase,
    IN ULONG64 ControlPc,
    IN PRUNTIME_FUNCTION FunctionEntry,
    IN OUT PCONTEXT ContextRecord,
    OUT PVOID *HandlerData,
    OUT PULONG64 EstablisherFrame,
    IN OUT PVOID ContextPointers OPTIONAL
    );

#define UNW_FLAG_NHANDLER 0x00

#elif defined (_IA64_)

#define PSR_RI      41
#define IPSR_RI_MASK ((ULONGLONG)0x3 << PSR_RI)      // PSR.ri (42:41)
#define PROGRAM_COUNTER_TO_CONTEXT(Context, ProgramCounter) \
    ((Context)->StIIP = (ProgramCounter) & ~(0x0fI64), \
     (Context)->StIPSR &= ~(IPSR_RI_MASK), \
     (Context)->StIPSR |= ((ProgramCounter) & 0x0cI64) << (PSR_RI-2), \
     (Context)->StIPSR = ((Context)->StIPSR & (IPSR_RI_MASK)) == (IPSR_RI_MASK) ? \
                         (Context)->StIPSR & ~(IPSR_RI_MASK) : (Context)->StIPSR )

typedef struct _FRAME_POINTERS {
    ULONGLONG MemoryStackFp;
    ULONGLONG BackingStoreFp;
} FRAME_POINTERS, *PFRAME_POINTERS;

PRUNTIME_FUNCTION
RtlLookupFunctionEntry (
    IN ULONGLONG ControlPc,
    OUT PULONGLONG ImageBase,
    OUT PULONGLONG TargetGp
    );

ULONGLONG
RtlVirtualUnwind (
    IN ULONGLONG ImageBase,
    IN ULONGLONG ControlPc,
    IN PRUNTIME_FUNCTION FunctionEntry,
    IN OUT PCONTEXT ContextRecord,
    OUT PBOOLEAN InFunction,
    OUT PFRAME_POINTERS EstablisherFrame,
    IN OUT PVOID ContextPointers OPTIONAL
    );

#endif  /* defined (_IA64_) */

typedef LONG NTSTATUS;
#define STATUS_STACK_BUFFER_OVERRUN      ((NTSTATUS)0xC0000409L)

extern UINT_PTR __security_cookie;
extern UINT_PTR __security_cookie_complement;

/*
 * Use global memory for the exception and context records, and any other local
 * state needed by __report_gsfailure.  We're not worried about multithread
 * issues here - we're going to die anyway, and it saves us from consuming a
 * good chunk of stack, as well as potentially overwriting useful data in the
 * stack memory dump.
 */

static EXCEPTION_RECORD         GS_ExceptionRecord;
static CONTEXT                  GS_ContextRecord;
static const EXCEPTION_POINTERS GS_ExceptionPointers = {
    &GS_ExceptionRecord,
    &GS_ContextRecord
};

#if defined (_CRTBLD) && !defined (_SYSCRT)
static BOOL DebuggerWasPresent;
#endif  /* defined (_CRTBLD) && !defined (_SYSCRT) */

/***
*__report_gsfailure() - Report security error.
*
*Purpose:
*       A /GS security error has been detected.  We save the registers in a
*       CONTEXT struct & call UnhandledExceptionFilter to display a message
*       to the user (invoke Watson handling) and terminate the program.
*
*       NOTE: __report_gsfailure should not be called directly.  Instead, it
*       should be entered due to a failure detected by __security_check_cookie.
*       That's because __security_check_cookie may perform more checks than
*       just a mismatch against the global security cookie, and because
*       the context captured by __report_gsfailure is unwound assuming that
*       __security_check_cookie was used to detect the failure.
*
*Entry:
*       ULONGLONG StackCookie - the local cookie, passed in as actual argument
*       on AMD64 and IA64 only.  On x86, the local cookie is available in ECX,
*       but since __report_gsfailure isn't __fastcall, it isn't a true
*       argument, and we must flush ECX to the context record quickly.
*
*Exit:
*       Does not return.
*
*Exceptions:
*
*******************************************************************************/

#pragma optimize("", off)   /* force an EBP frame on x86, no stack packing */

#if defined (_X86_)
__declspec(noreturn) void __cdecl __report_gsfailure(void)
#else  /* defined (_X86_) */
__declspec(noreturn) void __cdecl __report_gsfailure(ULONGLONG StackCookie)
#endif  /* defined (_X86_) */
{
    volatile UINT_PTR cookie[2];

    /*
     * Set up a fake exception, and report it via UnhandledExceptionFilter.
     * We can't raise a true exception because the stack (and therefore
     * exception handling) can't be trusted after a buffer overrun.  The
     * exception should appear as if it originated after the call to
     * __security_check_cookie, so it is attributed to the function where the
     * buffer overrun was detected.
     */

#if defined (_X86_)
    /*
     * On x86, we reserve some extra stack which won't be used.  That is to
     * preserve as much of the call frame as possible when the function with
     * the buffer overrun entered __security_check_cookie with a JMP instead of
     * a CALL, after the calling frame has been released in the epilogue of
     * that function.
     */

    volatile ULONG dw[(sizeof(CONTEXT) + sizeof(EXCEPTION_RECORD))/sizeof(ULONG)];

    /*
     * Save the state in the context record immediately.  Hopefully, since
     * opts are disabled, this will happen without modifying ECX, which has
     * the local cookie which failed the check.
     */

    __asm {
        mov dword ptr [GS_ContextRecord.Eax], eax
        mov dword ptr [GS_ContextRecord.Ecx], ecx
        mov dword ptr [GS_ContextRecord.Edx], edx
        mov dword ptr [GS_ContextRecord.Ebx], ebx
        mov dword ptr [GS_ContextRecord.Esi], esi
        mov dword ptr [GS_ContextRecord.Edi], edi
        mov word ptr [GS_ContextRecord.SegSs], ss
        mov word ptr [GS_ContextRecord.SegCs], cs
        mov word ptr [GS_ContextRecord.SegDs], ds
        mov word ptr [GS_ContextRecord.SegEs], es
        mov word ptr [GS_ContextRecord.SegFs], fs
        mov word ptr [GS_ContextRecord.SegGs], gs
        pushfd
        pop [GS_ContextRecord.EFlags]

        /*
         * Set the context EBP/EIP/ESP to the values which would be found
         * in the caller to __security_check_cookie.
         */

        mov eax, [ebp]
        mov dword ptr [GS_ContextRecord.Ebp], eax
        mov eax, [ebp+4]
        mov dword ptr [GS_ContextRecord.Eip], eax
        lea eax, [ebp+8]
        mov dword ptr [GS_ContextRecord.Esp], eax

        /*
         * Make sure the dummy stack space looks referenced.
         */

        mov eax, dword ptr dw
    }

    GS_ContextRecord.ContextFlags = CONTEXT_CONTROL;

    GS_ExceptionRecord.ExceptionAddress  = (PVOID)(ULONG_PTR)GS_ContextRecord.Eip;

#elif defined (_AMD64_)
    /*
     * On AMD64/EM64T, __security_check_cookie branched to __report_gsfailure
     * with a JMP, not CALL, so call RtlVirtualUnwind once to get the context
     * of the caller to __security_check_cookie, which is the function with
     * the buffer overrun.
     */

    ULONG64 ControlPc;
    ULONG64 EstablisherFrame;
    PRUNTIME_FUNCTION FunctionEntry;
    PVOID HandlerData;
    ULONG64 ImageBase;

    RtlCaptureContext(&GS_ContextRecord);
    ControlPc = GS_ContextRecord.Rip;
    FunctionEntry = RtlLookupFunctionEntry(ControlPc, &ImageBase, NULL);
    if (FunctionEntry != NULL) {
        RtlVirtualUnwind(UNW_FLAG_NHANDLER,
                         ImageBase,
                         ControlPc,
                         FunctionEntry,
                         &GS_ContextRecord,
                         &HandlerData,
                         &EstablisherFrame,
                         NULL);
    } else {
        GS_ContextRecord.Rip = (ULONGLONG) _ReturnAddress();
        GS_ContextRecord.Rsp = (ULONGLONG) _AddressOfReturnAddress()+8;
    }

    GS_ExceptionRecord.ExceptionAddress = (PVOID)GS_ContextRecord.Rip;
    GS_ContextRecord.Rcx = StackCookie;

#elif defined (_IA64_)
    /*
     * On IA64, __security_check_cookie branched to __report_gsfailure with a
     * true call, so call RtlVirtualUnwind twice to get the context of the
     * caller to __security_check_cookie, which is the function with the
     * buffer overrun.
     */

    ULONGLONG ControlPc;
    FRAME_POINTERS EstablisherFrame;
    PRUNTIME_FUNCTION FunctionEntry;
    BOOLEAN InFunction;
    ULONGLONG ImageBase;
    ULONGLONG TargetGp;
    int frames;

    RtlCaptureContext(&GS_ContextRecord);
    ControlPc = GS_ContextRecord.BrRp;

    for (frames = 0; frames < 2; ++frames)
    {
        FunctionEntry = RtlLookupFunctionEntry(ControlPc, &ImageBase, &TargetGp);
        if (FunctionEntry == NULL)
        {
            break;
        }
        ControlPc = RtlVirtualUnwind(ImageBase,
                                     ControlPc,
                                     FunctionEntry,
                                     &GS_ContextRecord,
                                     &InFunction,
                                     &EstablisherFrame,
                                     NULL);
        PROGRAM_COUNTER_TO_CONTEXT(&GS_ContextRecord, ControlPc);
    }

    GS_ExceptionRecord.ExceptionAddress  = (PVOID)ControlPc;
    GS_ContextRecord.IntV0 = StackCookie;

#else  /* defined (_IA64_) */
#error Unknown platform!
#endif  /* defined (_IA64_) */

    GS_ExceptionRecord.ExceptionCode  = STATUS_STACK_BUFFER_OVERRUN;
    GS_ExceptionRecord.ExceptionFlags = EXCEPTION_NONCONTINUABLE;

    /*
     * Save the global cookie and cookie complement locally - using an array
     * to defeat any potential stack-packing.
     */

    cookie[0] = __security_cookie;
    cookie[1] = __security_cookie_complement;

#if defined (_CRTBLD) && !defined (_SYSCRT)
    DebuggerWasPresent = IsDebuggerPresent();
    _CRT_DEBUGGER_HOOK(_CRT_DEBUGGER_GSFAILURE);
#endif  /* defined (_CRTBLD) && !defined (_SYSCRT) */

    /* Make sure any filter already in place is deleted. */
    SetUnhandledExceptionFilter(NULL);

    UnhandledExceptionFilter((EXCEPTION_POINTERS *)&GS_ExceptionPointers);

#if defined (_CRTBLD) && !defined (_SYSCRT)
    /*
     * If we make it back from Watson, then the user may have asked to debug
     * the app.  If we weren't under a debugger before invoking Watson,
     * re-signal the VS CRT debugger hook, so a newly attached debugger gets
     * a chance to break into the process.
     */
    if (!DebuggerWasPresent)
    {
        _CRT_DEBUGGER_HOOK(_CRT_DEBUGGER_GSFAILURE);
    }
#endif  /* defined (_CRTBLD) && !defined (_SYSCRT) */

    TerminateProcess(GetCurrentProcess(), STATUS_STACK_BUFFER_OVERRUN);
}
