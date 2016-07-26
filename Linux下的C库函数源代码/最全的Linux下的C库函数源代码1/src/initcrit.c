/***
*initcrit.c - CRT wrapper for InitializeCriticalSectionAndSpinCount
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains __crtInitCritSecAndSpinCount, a wrapper for
*       the Win32 API InitializeCriticalSectionAndSpinCount which is only
*       available on NT4SP3 or better.
*
*       *** For internal use only ***
*
*******************************************************************************/


#include <cruntime.h>
#include <windows.h>
#include <internal.h>
#include <rterr.h>
#include <stdlib.h>

typedef
BOOL
(WINAPI * PFN_INIT_CRITSEC_AND_SPIN_COUNT) (
    PCRITICAL_SECTION lpCriticalSection,
    DWORD dwSpinCount
);

static PFN_INIT_CRITSEC_AND_SPIN_COUNT __pfnInitCritSecAndSpinCount;

/***
*void _initp_misc_initcrit(void*)
*
*Purpose:
*       Initialize static pointer to function
*
*Entry:
*       The per-process encoded value for the NULL pointer.
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _initp_misc_initcrit(void* enull)
{
    __pfnInitCritSecAndSpinCount = (PFN_INIT_CRITSEC_AND_SPIN_COUNT) enull;
}

/***
*void __crtInitCritSecNoSpinCount() - InitializeCriticalSectionAndSpinCount
*                                     wrapper
*
*Purpose:
*       For systems where the Win32 API InitializeCriticalSectionAndSpinCount
*       is unavailable, this is called instead.  It just calls
*       InitializeCriticalSection and ignores the spin count.
*
*Entry:
*       PCRITICAL_SECTION lpCriticalSection - ptr to critical section
*       DWORD dwSpinCount - initial spin count setting
*
*Exit:
*       Always returns TRUE
*
*Exceptions:
*       InitializeCriticalSection can raise a STATUS_NO_MEMORY exception.
*
*******************************************************************************/

static BOOL WINAPI __crtInitCritSecNoSpinCount (
    PCRITICAL_SECTION lpCriticalSection,
    DWORD dwSpinCount
    )
{
    InitializeCriticalSection(lpCriticalSection);
    return TRUE;
}

/***
*int __crtInitCritSecAndSpinCount() - initialize critical section
*
*Purpose:
*       Calls InitializeCriticalSectionAndSpinCount, if available, otherwise
*       InitializeCriticalSection.  On multiprocessor systems, a spin count
*       should be used with critical sections, but the appropriate APIs are
*       only available on NT4SP3 or later.
*
*       Also handles the out of memory condition which is possible with
*       InitializeCriticalSection[AndSpinCount].
*
*Entry:
*       PCRITICAL_SECTION lpCriticalSection - ptr to critical section
*       DWORD dwSpinCount - initial spin count setting
*
*Exit:
*       Returns FALSE and sets Win32 last-error code to ERROR_NOT_ENOUGH_MEMORY
*       if InitializeCriticalSection[AndSpinCount] fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __crtInitCritSecAndSpinCount (
    PCRITICAL_SECTION lpCriticalSection,
    DWORD dwSpinCount
    )
{
    int ret;
    unsigned int osplatform = 0;
    PFN_INIT_CRITSEC_AND_SPIN_COUNT pfnInitCritSecAndSpinCount = (PFN_INIT_CRITSEC_AND_SPIN_COUNT) _decode_pointer(__pfnInitCritSecAndSpinCount);

    if (pfnInitCritSecAndSpinCount == NULL) {
        /*
         * First time through, see if InitializeCriticalSectionAndSpinCount
         * is available.  If not, use a wrapper over InitializeCriticalSection
         * instead.
         */
        _ERRCHECK(_get_osplatform(&osplatform));
        if (osplatform == VER_PLATFORM_WIN32_WINDOWS) {
            /*
             * Win98 and WinME export InitializeCriticalSectionAndSpinCount,
             * but it is non-functional (it should return a BOOL, but is
             * VOID instead, returning a useless return value).  Use the
             * dummy API instead.
             */
            pfnInitCritSecAndSpinCount = __crtInitCritSecNoSpinCount;
        } else {
            HINSTANCE hKernel32 = GetModuleHandle("kernel32.dll");
            if (hKernel32 != NULL) {
                pfnInitCritSecAndSpinCount = (PFN_INIT_CRITSEC_AND_SPIN_COUNT)
                    GetProcAddress(hKernel32,
                                   "InitializeCriticalSectionAndSpinCount");

                if (pfnInitCritSecAndSpinCount == NULL) {
                    /*
                     * InitializeCriticalSectionAndSpinCount not available,
                     * use dummy API
                     */
                    pfnInitCritSecAndSpinCount = __crtInitCritSecNoSpinCount;
                }
            }
            else {
                /*
                 * GetModuleHandle failed (should never happen),
                 * use dummy API
                 */
                pfnInitCritSecAndSpinCount = __crtInitCritSecNoSpinCount;
            }
        }
        __pfnInitCritSecAndSpinCount = (PFN_INIT_CRITSEC_AND_SPIN_COUNT) _encode_pointer(pfnInitCritSecAndSpinCount);
    }

    __try {
        /*
         * Call the real InitializeCriticalSectionAndSpinCount, or the
         * wrapper which just calls InitializeCriticalSection if the newer
         * API is not available.
         */
        ret = pfnInitCritSecAndSpinCount(lpCriticalSection, dwSpinCount);
    }
    __except (_exception_code()== STATUS_NO_MEMORY ? EXCEPTION_EXECUTE_HANDLER:EXCEPTION_CONTINUE_SEARCH) {
        /*
         * Initialization failed by raising an exception, which is probably
         * STATUS_NO_MEMORY.  It is not safe to set the CRT errno to ENOMEM,
         * since the per-thread data may not yet exist.  Instead, set the Win32
         * error which can be mapped to ENOMEM later.
         */
        if (GetExceptionCode() == STATUS_NO_MEMORY) {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        }
        ret = FALSE;
    }

    return ret;
}

