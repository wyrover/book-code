/***
*secchk.c - checks buffer overrun security cookie for x86
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines compiler helper __security_check_cookie, used by the /GS
*       compile switch to detect local buffer variable overrun bugs/attacks.
*
*       When compiling /GS, the compiler injects code to detect when a local
*       array variable has been overwritten, potentially overwriting the
*       return address (on machines like x86 where the return address is on
*       the stack).  A local variable is allocated directly before the return
*       address and initialized on entering the function.  When exiting the
*       function, the compiler inserts code to verify that the local variable
*       has not been modified.  If it has, then an error reporting routine
*       is called.
*
*******************************************************************************/

#include <windows.h>
#include <process.h>

/*
 * The global security cookie.  This name is known to the compiler.
 */
extern UINT_PTR __security_cookie;

/***
*__security_check_cookie(cookie) - check for buffer overrun
*
*Purpose:
*       Compiler helper.  Check if a local copy of the security cookie still
*       matches the global value.  If not, then report the fatal error.
*
*       The actual reporting is done by __report_gsfailure
*       since the cookie check routine must be minimal code that preserves
*       any registers used in returning the callee's result.
*
*Entry:
*       UINT_PTR cookie - local security cookie to check
*
*Exit:
*       Returns immediately if the local cookie matches the global version.
*       Otherwise, calls the failure reporting handler and exits.
*
*Exceptions:
*
*******************************************************************************/

void __declspec(naked) __fastcall __security_check_cookie(UINT_PTR cookie)
{
    /* x86 version written in asm to preserve all regs */
    __asm {
        cmp ecx, __security_cookie
        jne failure
        rep ret /* REP to avoid AMD branch prediction penalty */
failure:
        jmp __report_gsfailure
    }
}
