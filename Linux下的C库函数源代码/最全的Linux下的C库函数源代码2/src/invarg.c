/***
*invarg.c - stub for invalid argument handler
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines _invalidarg() and _set_invalid_parameter_handler()
*
*
*******************************************************************************/

#include <cruntime.h>
#include <sect_attribs.h>
#include <internal.h>
#include <rterr.h>
#include <stdlib.h>
#include <windows.h>
#include <dbgint.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void * _ReturnAddress(void);
#pragma intrinsic(_ReturnAddress)

#ifdef _X86_
void * _AddressOfReturnAddress(void);
#pragma intrinsic(_AddressOfReturnAddress)
#endif  /* _X86_ */

/* global variable which stores the user handler */

_invalid_parameter_handler __pInvalidArgHandler;

extern "C"
void _initp_misc_invarg(void* enull)
{
    __pInvalidArgHandler = (_invalid_parameter_handler) enull;
}

/***
*void _invalid_arg() -
*
*Purpose:
*   Called when an invalid argument is passed into a CRT function
*
*Entry:
*   pszExpression - validation expression (can be NULL)
*   pszFunction - function name (can be NULL)
*   pszFile - file name (can be NULL)
*   nLine - line number
*   pReserved - reserved for future use
*
*Exit:
*   return from this function results in error being returned from the function
*   that called the handler
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void __cdecl _invalid_parameter(
    const wchar_t *pszExpression,
    const wchar_t *pszFunction,
    const wchar_t *pszFile,
    unsigned int nLine,
    uintptr_t pReserved
    )
{
    _invalid_parameter_handler pHandler = __pInvalidArgHandler;

    pszExpression;
    pszFunction;
    pszFile;

    pHandler = (_invalid_parameter_handler) _decode_pointer(pHandler);
    if (pHandler != NULL)
    {
        pHandler(pszExpression, pszFunction, pszFile, nLine, pReserved);
        return;
    }

    // No user handler is defined. Notify the debugger if attached.

    _CRT_DEBUGGER_HOOK(_CRT_DEBUGGER_INVALIDPARAMETER);

    _invoke_watson(pszExpression, pszFunction, pszFile, nLine, pReserved);
}

#ifndef _DEBUG

/* wrapper which passes no debug info; not available in debug
 * used in inline code: we don't pass the null params, so we gain some
 * speed and space in code generation
 */
_CRTIMP void __cdecl _invalid_parameter_noinfo(void)
{
    _invalid_parameter(NULL, NULL, NULL, 0, 0);
}

#endif  /* _DEBUG */

_CRTIMP void __cdecl _invoke_watson(
    const wchar_t *pszExpression,
    const wchar_t *pszFunction,
    const wchar_t *pszFile,
    unsigned int nLine,
    uintptr_t pReserved
    )
{
    /* Fake an exception to call reportfault. */
    EXCEPTION_RECORD ExceptionRecord;
    CONTEXT ContextRecord;
    EXCEPTION_POINTERS ExceptionPointers;
    BOOL wasDebuggerPresent = FALSE;
    DWORD ret = 0;

    (pszExpression);
    (pszFunction);
    (pszFile);
    (nLine);
    (pReserved);

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

    /* Need to fill up the Context in IA64 and AMD64. */
    RtlCaptureContext(&ContextRecord);

#else  /* defined (_IA64_) || defined (_AMD64_) */

    ZeroMemory(&ContextRecord, sizeof(ContextRecord));

#endif  /* defined (_IA64_) || defined (_AMD64_) */

    ZeroMemory(&ExceptionRecord, sizeof(ExceptionRecord));

    ExceptionRecord.ExceptionCode = STATUS_INVALID_PARAMETER;
    ExceptionRecord.ExceptionAddress = _ReturnAddress();

    ExceptionPointers.ExceptionRecord = &ExceptionRecord;
    ExceptionPointers.ContextRecord = &ContextRecord;

    wasDebuggerPresent = IsDebuggerPresent();

    /* Make sure any filter already in place is deleted. */
    SetUnhandledExceptionFilter(NULL);

    ret = UnhandledExceptionFilter(&ExceptionPointers);

    // if no handler found and no debugger previously attached
    // the execution must stop into the debugger hook.
    if (ret == EXCEPTION_CONTINUE_SEARCH && !wasDebuggerPresent) {
        _CRT_DEBUGGER_HOOK(_CRT_DEBUGGER_INVALIDPARAMETER);
    }

    TerminateProcess(GetCurrentProcess(), STATUS_INVALID_PARAMETER);
}

/***
*void _set_invalid_parameter_handler(void) -
*
*Purpose:
*       Establish a handler to be called when a CRT detects a invalid parameter
*
*       This function is not thread-safe
*
*Entry:
*   New handler
*
*Exit:
*   Old handler
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP _invalid_parameter_handler __cdecl
_set_invalid_parameter_handler( _invalid_parameter_handler pNew )
{
    _invalid_parameter_handler pOld = NULL;

    pOld = __pInvalidArgHandler;
    pOld = (_invalid_parameter_handler) _decode_pointer((PVOID)pOld);
    pNew = (_invalid_parameter_handler) _encode_pointer((PVOID)pNew);

    __pInvalidArgHandler = pNew;

    return pOld;
}

_CRTIMP _invalid_parameter_handler __cdecl
_get_invalid_parameter_handler( )
{
    _invalid_parameter_handler pOld = NULL;

    pOld = __pInvalidArgHandler;
    pOld = (_invalid_parameter_handler) _decode_pointer((PVOID)pOld);

    return pOld;
}

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef __cplusplus

#if defined (_NATIVE_WCHAR_T_DEFINED)
/*
unsigned short -> wchar_t
This is defined as a real function rather than an alias because the parameter types
are different in the two versions which means different metadata is required when building /clr apps.
*/
extern "C++" _CRTIMP void __cdecl _invalid_parameter(
    const unsigned short * pszExpression,
    const unsigned short * pszFunction,
    const unsigned short * pszFile,
    unsigned int nLine,
    uintptr_t pReserved
    )
{
    _invalid_parameter(
        reinterpret_cast<const wchar_t *>(pszExpression),
        reinterpret_cast<const wchar_t *>(pszFunction),
        reinterpret_cast<const wchar_t *>(pszFile),
        nLine,
        pReserved
        );
}

extern "C++" void __cdecl _invoke_watson(
    const unsigned short * pszExpression,
    const unsigned short * pszFunction,
    const unsigned short * pszFile,
    unsigned int nLine,
    uintptr_t pReserved
    )
{
    _invoke_watson(
        reinterpret_cast<const wchar_t *>(pszExpression),
        reinterpret_cast<const wchar_t *>(pszFunction),
        reinterpret_cast<const wchar_t *>(pszFile),
        nLine,
        pReserved
        );
}

#endif  /* defined (_NATIVE_WCHAR_T_DEFINED) */

#endif  /* __cplusplus */

