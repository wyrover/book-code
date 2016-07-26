/***
*tlsdyn.c - Thread Local Storage dynamic initialization run-time support module
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Provide the TLS equivalent of DllMainCRTStartup, to be called by the
*       OS when a thread begins or ends.  On thread start, walk the list of
*       pointers to initialization routines for dynamically-initialized
*       __declspec(thread) variables.  On thread stop, walk the list of
*       destructors for __declspec(thread) variables, as registered at
*       initialization time.
*
****/

#ifdef _MSC_VER

#ifndef MRTDLL

#ifdef CRTDLL
#undef CRTDLL
#endif  /* CRTDLL */

#include <cruntime.h>
#include <internal.h>
#include <sect_attribs.h>
#include <windows.h>

/*
 * Start and end sections for array of TLS callBacks to initialize individual
 * dynamically initialized __declspec(thread) items.  The actual array of
 * callbacks is constructed using .CRT$XDA, .CRT$XDC, .CRT$XDL, .CRT$XDU,
 * and .CRT$XDZ similar to the way global static initializers are done
 * for C++.  The C++ compiler will inject function pointers into .CRT$XD[CLU]
 * for each dynamically initialized __declspec(thread) variable, as well as
 * injecting a /include:__dyn_tls_init directive into the enclosing .obj,
 * to force inclusion of this support object.
 */

static _CRTALLOC(".CRT$XDA") _PVFV __xd_a = 0;

static _CRTALLOC(".CRT$XDZ") _PVFV __xd_z = 0;

/*
 * __dyn_tls_init - dynamically initialize __declspec(thread) variables
 *
 * Purpose:
 *      When any thread starts up, walk the array of function pointers found
 *      in sections .CRT$XD*, calling each non-NULL entry to dynamically
 *      initialize that thread's copy of a __declspec(thread) variable.
 *
 * Entry:
 *      This is called directly from the Windows loader code, with dwReason
 *      equal to one of DLL_PROCESS_ATTACH/DETACH or DLL_THREAD_ATTACH_DETACH.
 *
 * Exit:
 *      Returns TRUE always, though the loader code ignores the result.
 *
 * Notes:
 *      Only calls the initializers on DLL_THREAD_ATTACH and not
 *      DLL_PROCESS_ATTACH.  That's because the DLL_PROCESS_ATTACH call happens
 *      too early, before the CRT has been initialized in, e.g.,
 *      DllMainCRTStartup or mainCRTStartup.  Instead, the CRT init code will
 *      call here directly with a DLL_THREAD_ATTACH to initialize any
 *      __declspec(thread) variables in the primary thread at process startup.
 */

BOOL WINAPI __dyn_tls_init(
    HANDLE  hDllHandle,
    DWORD   dwReason,
    LPVOID  lpreserved
    )
{
    _PVFV *pfunc;

    if (dwReason != DLL_THREAD_ATTACH) {
        return TRUE;
    }

    /* prefast assumes we are overflowing __xd_a */
#pragma warning(push)
#pragma warning(disable:26000)
    for (pfunc = &__xd_a + 1; pfunc != &__xd_z; ++pfunc) {
        if (*pfunc != NULL) {
            (*pfunc)();
        }
    }
#pragma warning(pop)

    return TRUE;
}

/*
 * Define an initialized callback function pointer, so CRT startup code knows
 * we have dynamically initialized __declspec(thread) variables that need to
 * be initialized at process startup for the primary thread.
 */

const PIMAGE_TLS_CALLBACK __dyn_tls_init_callback = __dyn_tls_init;

/*
 * Enter a callback function pointer into the .CRT$XL* array, which is the
 * callback array pointed to by the IMAGE_TLS_DIRECTORY in the PE header, so
 * the OS knows we want to be notified on each thread startup/shutdown.
 */

static _CRTALLOC(".CRT$XLC") PIMAGE_TLS_CALLBACK __xl_c = __dyn_tls_init;

#endif  /* MRTDLL */

#endif  /* _MSC_VER */
