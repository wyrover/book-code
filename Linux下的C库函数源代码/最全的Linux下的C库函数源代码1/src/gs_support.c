/***
*gs_support.c - initialize the global buffer overrun security cookie
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Define __security_init_cookie, which is called at startup to initialize
*       the global buffer overrun security cookie used by the /GS compile flag.
*
*******************************************************************************/

#include <windows.h>
#include <intrin.h>

#if defined (_M_IX86) && defined (_CRTBLD) && !defined (_SYSCRT) && defined (_DEBUG)
/*
 * __security_init_cookie must be called before any exception handlers using
 * the cookie are registered.  We do a spot check for this condition in the
 * debug version of the x86 CRT.
 */

#define CHECK_FOR_LATE_COOKIE_INIT

#include <cmsgs.h>  // for _RT_COOKIE_INIT_TXT

typedef
EXCEPTION_DISPOSITION
(*PEXCEPTION_ROUTINE) (
    IN struct _EXCEPTION_RECORD *ExceptionRecord,
    IN PVOID EstablisherFrame,
    IN OUT struct _CONTEXT *ContextRecord,
    IN OUT PVOID DispatcherContext
    );

typedef struct _EXCEPTION_REGISTRATION_RECORD {
    struct _EXCEPTION_REGISTRATION_RECORD *Next;
    PEXCEPTION_ROUTINE Handler;
} EXCEPTION_REGISTRATION_RECORD;

typedef EXCEPTION_REGISTRATION_RECORD *PEXCEPTION_REGISTRATION_RECORD;

#define EXCEPTION_CHAIN_END ((struct _EXCEPTION_REGISTRATION_RECORD * POINTER_32)-1)

EXCEPTION_DISPOSITION __cdecl
_except_handler4(
    IN struct _EXCEPTION_RECORD *ExceptionRecord,
    IN PVOID EstablisherFrame,
    IN OUT struct _CONTEXT *ContextRecord,
    IN OUT PVOID DispatcherContext
    );

#endif  /* defined (_M_IX86) && defined (_CRTBLD) && !defined (_SYSCRT) && defined (_DEBUG) */

/*
 * Default value used for the global /GS security cookie, defined here and
 * in gs_cookie.c (since standalone SDK build can't use CRT's internal.h).
 */

#ifdef _WIN64
#define DEFAULT_SECURITY_COOKIE 0x00002B992DDFA232
#else  /* _WIN64 */
#define DEFAULT_SECURITY_COOKIE 0xBB40E64E
#endif  /* _WIN64 */

/*
 * The global security cookie.  This name is known to the compiler.
 */
extern UINT_PTR __security_cookie;
extern UINT_PTR __security_cookie_complement;

/*
 * Union to facilitate converting from FILETIME to unsigned __int64
 */
typedef union {
    unsigned __int64 ft_scalar;
    FILETIME ft_struct;
} FT;

/***
*__security_init_cookie(cookie) - init buffer overrun security cookie.
*
*Purpose:
*       Initialize the global buffer overrun security cookie which is used by
*       the /GS compile switch to detect overwrites to local array variables
*       the potentially corrupt the return address.  This routine is called
*       at EXE/DLL startup.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl __security_init_cookie(void)
{
    UINT_PTR cookie;
    FT systime={0};
    LARGE_INTEGER perfctr;
#if defined (CHECK_FOR_LATE_COOKIE_INIT)
    PEXCEPTION_REGISTRATION_RECORD ehrec;
#endif  /* defined (CHECK_FOR_LATE_COOKIE_INIT) */

    /*
     * Do nothing if the global cookie has already been initialized.  On x86,
     * reinitialize the cookie if it has been previously initialized to a
     * value with the high word 0x0000.  Some versions of Windows will init
     * the cookie in the loader, but using an older mechanism which forced the
     * high word to zero.
     */

    if (__security_cookie != DEFAULT_SECURITY_COOKIE
#if defined (_X86_)
        && (__security_cookie & 0xFFFF0000) != 0
#endif  /* defined (_X86_) */
       )
    {
        __security_cookie_complement = ~__security_cookie;
        return;
    }

#if defined (CHECK_FOR_LATE_COOKIE_INIT)
    /*
     * The security cookie is used to ensure the integrity of exception
     * handling.  That means the cookie must be initialized in an image before
     * that image registers an exception handler that will use the cookie.  We
     * attempt to detect this situation by checking if the calling thread has
     * already registered SEH handler _except_handler4.
     */

    for (ehrec = (PEXCEPTION_REGISTRATION_RECORD)(UINT_PTR)
                    __readfsdword(FIELD_OFFSET(NT_TIB, ExceptionList));
         ehrec != EXCEPTION_CHAIN_END;
         ehrec = ehrec->Next)
    {
        if (ehrec->Handler == &_except_handler4)
        {
            /*
             * We've detected __except_handler4, alert the user by issuing
             * error R6035 and terminate the process.  We use FatalAppExit
             * instead of _NMSG_WRITE because the loader lock may be held now,
             * and _NMSG_WRITE may do GetLibrary("user32.dll") for MessageBox,
             * which is unsafe under the loader lock.
             */

            FatalAppExit(0, _RT_COOKIE_INIT_TXT);
        }

        if (ehrec >= ehrec->Next)
        {
            /*
             * Nodes in the exception record linked list are supposed to appear
             * at increasing addresses within the thread's stack.  If the next
             * node is at a lower address, then the exception list is corrupted,
             * and we stop our check to avoid hitting a cycle in the list.
             */

            break;
        }
    }
#endif  /* defined (CHECK_FOR_LATE_COOKIE_INIT) */

    /*
     * Initialize the global cookie with an unpredictable value which is
     * different for each module in a process.  Combine a number of sources
     * of randomness.
     */

    GetSystemTimeAsFileTime(&systime.ft_struct);
#if defined (_WIN64)
    cookie = systime.ft_scalar;
#else  /* defined (_WIN64) */
    cookie = systime.ft_struct.dwLowDateTime;
    cookie ^= systime.ft_struct.dwHighDateTime;
#endif  /* defined (_WIN64) */

    cookie ^= GetCurrentProcessId();
    cookie ^= GetCurrentThreadId();
    cookie ^= GetTickCount();

    QueryPerformanceCounter(&perfctr);
#if defined (_WIN64)
    cookie ^= perfctr.QuadPart;
#else  /* defined (_WIN64) */
    cookie ^= perfctr.LowPart;
    cookie ^= perfctr.HighPart;
#endif  /* defined (_WIN64) */

#if defined (_WIN64)
    /*
     * On Win64, generate a cookie with the most significant word set to zero,
     * as a defense against buffer overruns involving null-terminated strings.
     * Don't do so on Win32, as it's more important to keep 32 bits of cookie.
     */
    cookie &= 0x0000FFFFffffFFFFi64;
#endif  /* defined (_WIN64) */

    /*
     * Make sure the cookie is initialized to a value that will prevent us from
     * reinitializing it if this routine is ever called twice.
     */

    if (cookie == DEFAULT_SECURITY_COOKIE)
    {
        cookie = DEFAULT_SECURITY_COOKIE + 1;
    }
#if defined (_X86_)
    else if ((cookie & 0xFFFF0000) == 0)
    {
        cookie |= cookie << 16;
    }
#endif  /* defined (_X86_) */

    __security_cookie = cookie;
    __security_cookie_complement = ~cookie;

}
