/***
*crtexe.c - Initialization for console EXE using CRT DLL
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for apps linking to the CRT DLL.
*       It calls the user's main routine [w]main() or [w]WinMain after
*       performing C Run-Time Library initialization.
*
*       With ifdefs, this source file also provides the source code for:
*       wcrtexe.c   the startup routine for console apps with wide chars
*       crtexew.c   the startup routine for Windows apps
*       wcrtexew.c  the startup routine for Windows apps with wide chars
*
*******************************************************************************/

#ifdef CRTDLL

/*
 * SPECIAL BUILD MACROS! Note that crtexe.c (and crtexew.c) is linked in with
 * the client's code. It does not go into crtdll.dll! Therefore, it must be
 * built under the _DLL switch (like user code) and CRTDLL must be undefined.
 * The symbol SPECIAL_CRTEXE is turned on to suppress the normal CRT DLL
 * definition of _fmode and _commode using __declspec(dllexport).  Otherwise
 * this module would not be able to refer to both the local and DLL versions
 * of these two variables.
 */

#undef  CRTDLL
#define _DLL

/*
 * We need the manifest information in crtexe.obj so that user exe's which
 * do not include any headers get a manifest
 */
#undef _CRT_NOFORCE_MANIFEST
#undef _VC_NODEFAULTLIB

#define SPECIAL_CRTEXE

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <process.h>
#include <math.h>
#include <rterr.h>
#include <stdlib.h>
#include <tchar.h>
#include <rtcapi.h>
#include <sect_attribs.h>
#include <locale.h>

#if defined (_WIN64) && defined (_M_IA64)
#pragma section(".base", long, read)
__declspec(allocate(".base"))
extern
IMAGE_DOS_HEADER __ImageBase;
#else  /* defined (_WIN64) && defined (_M_IA64) */
extern
IMAGE_DOS_HEADER __ImageBase;
#endif  /* defined (_WIN64) && defined (_M_IA64) */

/*
 * wWinMain is not yet defined in winbase.h. When it is, this should be
 * removed.
 */

int
WINAPI
wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nShowCmd
    );

#define SPACECHAR   _T(' ')
#define DQUOTECHAR  _T('\"')

#ifdef _M_IX86

/*
 * The local copy of the Pentium FDIV adjustment flag
 * and the address of the flag in MSVCRT*.DLL.
 */

extern int _adjust_fdiv;

extern int * _imp___adjust_fdiv;

#endif  /* _M_IX86 */


/* default floating point precision - X86 only! */

#ifdef _M_IX86
extern void _setdefaultprecision();
#endif  /* _M_IX86 */


/*
 * Declare function used to install a user-supplied _matherr routine.
 */
_CRTIMP void __setusermatherr( int (__cdecl *)(struct _exception *) );


/*
 * Declare the names of the exports corresponding to _fmode and _commode
 */
#ifdef _M_IX86

#define _IMP___FMODE    (__p__fmode())
#define _IMP___COMMODE  (__p__commode())

#else  /* _M_IX86 */

/* assumed to be MIPS or Alpha */

#define _IMP___FMODE    __imp__fmode
#define _IMP___COMMODE  __imp__commode

#endif  /* _M_IX86 */

#if !defined (_M_IX86)
extern int * _IMP___FMODE;      /* exported from the CRT DLL */
extern int * _IMP___COMMODE;    /* these names are implementation-specific */
#endif  /* !defined (_M_IX86) */

extern int _fmode;          /* must match the definition in <stdlib.h> */
extern int _commode;        /* must match the definition in <internal.h> */
extern int _dowildcard;     /* passed to __getmainargs() */

/*
 * Declare/define communal that serves as indicator the default matherr
 * routine is being used.
 */
int __defaultmatherr;

/*
 * routine in DLL to do initialization (in this case, C++ constructors)
 */
extern int __cdecl _initterm_e(_PIFV *, _PIFV *);
extern void __cdecl _initterm(_PVFV *, _PVFV *);

/*
 * routine to check if this is a managed application
 */
static int __cdecl check_managed_app(void);

/*
 * pointers to initialization sections
 */
extern _CRTALLOC(".CRT$XIA") _PIFV __xi_a[];
extern _CRTALLOC(".CRT$XIZ") _PIFV __xi_z[];    /* C initializers */
extern _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */

/*
 * Pointer to callback function to initialize any dynamically initialized
 * __declspec(thread) variables.  This relies on a subtle aspect of C.
 * The pointer is defined here uninitialized.  It is defined initialized in
 * tlsdyn.c.  If user code uses dynamically initialized __declspec(thread)
 * variables, then compiler-injected dependencies cause tlsdyn.obj to be
 * linked.  In that case, the non-zero definition of __dyn_tls_init_callback
 * in tlsdyn.obj will take precedence, and the startup code will execute the
 * callback.  This use of multiple definitions is only legal in C, not C++.
 */

const PIMAGE_TLS_CALLBACK __dyn_tls_init_callback;

/*
 * Pointers to beginning and end of the table of function pointers manipulated
 * by _onexit()/atexit().  The atexit/_onexit code is shared for both EXE's and
 * DLL's but different behavior is required.  These values are set to -1 to
 * mark this module as an EXE.
 * NOTE - the pointers are stored encoded.
 */

extern _PVFV *__onexitbegin;
extern _PVFV *__onexitend;

/*
 * All the below variables are made static global for this file. This facilitates
 * in communicating mainCRTStartup and initialization code. Along with making these
 * variables global, two new functions are created pre_c_init, pre_cpp_init. The functions
 * are executed before anything in c_init and cpp_init sections repsectively.
 */

static int argc;   /* three standard arguments to main */
static _TSCHAR **argv;
static _TSCHAR **envp;

static int argret;
static int mainret=0;
static int managedapp;
static int has_cctor = 0;
static _startupinfo    startinfo;

static int __cdecl pre_c_init(void);
static void __cdecl pre_cpp_init(void);

_CRTALLOC(".CRT$XIAA") static _PIFV pcinit = pre_c_init;
_CRTALLOC(".CRT$XCAA") static _PVFV pcppinit = pre_cpp_init;

/***
*pre_c_init(void)
*
*Purpose:
*       The code in mainCRTStartup that was executed before executing C
*       initializers was shifted in this function. Also this funciton is the
*       first thing that is executed in c init section.
*
*Entry:
*
*Exit:
*
*******************************************************************************/

static int __cdecl pre_c_init(void)
{
    /*
     * Determine if this is a managed application
     */
    managedapp = check_managed_app();

    /*
     * Set __app_type properly
     */
#ifdef _WINMAIN_
    __set_app_type(_GUI_APP);
#else  /* _WINMAIN_ */
    __set_app_type(_CONSOLE_APP);
#endif  /* _WINMAIN_ */

    /*
     * Mark this module as an EXE file so that atexit/_onexit
     * will do the right thing when called, including for C++
     * d-tors.
     */
    __onexitbegin = __onexitend = (_PVFV *)_encode_pointer((_PVFV *)(-1));

    /*
     * Propogate the _fmode and _commode variables to the DLL
     */
    *_IMP___FMODE = _fmode;
    *_IMP___COMMODE = _commode;

#ifdef _M_IX86
    /*
     * Set the local copy of the Pentium FDIV adjustment flag
     */

    _adjust_fdiv = * _imp___adjust_fdiv;
#endif  /* _M_IX86 */

    /*
     * Run the RTC initialization code for this DLL
     */
#ifdef _RTC
    _RTC_Initialize();
#endif  /* _RTC */

    /*
     * Call _setargv(), which will trigger a call to __setargv() if
     * SETARGV.OBJ is linked with the EXE.  If SETARGV.OBJ is not
     * linked with the EXE, a dummy _setargv() will be called.
     */
#ifdef WPRFLAG
    _wsetargv();
#else  /* WPRFLAG */
    _setargv();
#endif  /* WPRFLAG */

    /*
     * If the user has supplied a _matherr routine then set
     * __pusermatherr to point to it.
     */
    if ( !__defaultmatherr )
        __setusermatherr(_matherr);

#ifdef _M_IX86
    _setdefaultprecision();
#endif  /* _M_IX86 */

    /* Enable per-thread locale if user asked for it */
    if(__globallocalestatus == -1)
    {
        _configthreadlocale(-1);
    }

    return 0;
}

/***
*pre_cpp_init(void)
*
*Purpose:
*       The code in mainCRTStartup that was executed after C initializers and
*       before C++ initializers is shifted in this function. Also this funciton
*       is the first thing that is executed in C++ init section.
*
*Entry:
*
*Exit:
*
*******************************************************************************/

static void __cdecl pre_cpp_init(void)
{
#ifdef _RTC
    atexit(_RTC_Terminate);
#endif  /* _RTC */

    /*
     * Get the arguments for the call to main. Note this must be
     * done explicitly, rather than as part of the dll's
     * initialization, to implement optional expansion of wild
     * card chars in filename args
     */

    startinfo.newmode = _newmode;


#ifdef WPRFLAG
    argret = __wgetmainargs(&argc, &argv, &envp,
                            _dowildcard, &startinfo);
#else  /* WPRFLAG */
    argret = __getmainargs(&argc, &argv, &envp,
                           _dowildcard, &startinfo);
#endif  /* WPRFLAG */

#ifndef _SYSCRT
    if (argret < 0)
        _amsg_exit(_RT_SPACEARG);
#endif  /* _SYSCRT */
}

/***
*mainCRTStartup(void)
*wmainCRTStartup(void)
*WinMainCRTStartup(void)
*wWinMainCRTStartup(void)
*
*Purpose:
*       These routines do the C runtime initialization, call the appropriate
*       user entry function, and handle termination cleanup.  For a managed
*       app, they then return the exit code back to the calling routine, which
*       is the managed startup code.  For an unmanaged app, they call exit and
*       never return.
*
*       Function:               User entry called:
*       mainCRTStartup          main
*       wmainCRTStartup         wmain
*       WinMainCRTStartup       WinMain
*       wWinMainCRTStartup      wWinMain
*
*Entry:
*
*Exit:
*       Managed app: return value from main() et al, or the exception code if
*                 execution was terminated by the __except guarding the call
*                 to main().
*       Unmanaged app: never return.
*
*******************************************************************************/

static
int
__tmainCRTStartup(
         void
         );

#ifdef _WINMAIN_

#ifdef WPRFLAG
int wWinMainCRTStartup(
#else  /* WPRFLAG */
int WinMainCRTStartup(
#endif  /* WPRFLAG */

#else  /* _WINMAIN_ */

#ifdef WPRFLAG
int wmainCRTStartup(
#else  /* WPRFLAG */
int mainCRTStartup(
#endif  /* WPRFLAG */

#endif  /* _WINMAIN_ */
        void
        )
{
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
        __security_init_cookie();

        return __tmainCRTStartup();
}

__declspec(noinline)
int
__tmainCRTStartup(
        void
        )
{
#ifdef _WINMAIN_
        _TUCHAR *lpszCommandLine;
        STARTUPINFO StartupInfo;
        BOOL inDoubleQuote=FALSE;

        __try {
                        /*
                        Note: MSDN specifically notes that GetStartupInfo returns no error, and throws unspecified SEH if it fails, so
                        the very general exception handler below is appropriate
                        */
            GetStartupInfo( &StartupInfo );
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            return 255;
        }
#endif  /* _WINMAIN_ */


        /*
         * Guard the initialization code and the call to user's main, or
         * WinMain, function in a __try/__except statement.
         */

        __try
        {
            /*
             * There is a possiblity that the module where this object is
             * linked into is a mixed module. In all the cases we gurantee that
             * native initialization will occur before managed initialization.
             * Also in anycase this code should never be called when some other
             * code is initializing native code, that's why we exit in that case.
             *
             * Do runtime startup initializers.
             *
             * Note: the only possible entry we'll be executing here is for
             * __lconv_init, pulled in from charmax.obj only if the EXE was
             * compiled with -J.  All other .CRT$XI* initializers are only
             * run as part of the CRT itself, and so for the CRT DLL model
             * are not found in the EXE.  For that reason, we call _initterm,
             * not _initterm_e, because __lconv_init will never return failure,
             * and _initterm_e is not exported from the CRT DLL.
             *
             * Note further that, when using the CRT DLL, executing the
             * .CRT$XI* initializers is only done for an EXE, not for a DLL
             * using the CRT DLL.  That is to make sure the -J setting for
             * the EXE is not overriden by that of any DLL.
             */
            void *lock_free=0;
            void *fiberid=((PNT_TIB)NtCurrentTeb())->StackBase;
            int nested=FALSE;
            while((lock_free=InterlockedCompareExchangePointer((volatile PVOID *)&__native_startup_lock, fiberid, 0))!=0)
            {
                if(lock_free==fiberid)
                {
                    nested=TRUE;
                    break;
                }

                /* some other thread is running native startup/shutdown during a cctor/domain unload.
                    Should only happen if this DLL was built using the Everett-compat loader lock fix in vcclrit.h
                */
                /* wait for the other thread to complete init before we return */
                Sleep(1000);
            }

            if (__native_startup_state == __initializing)
            {
                _amsg_exit( _RT_CRT_INIT_CONFLICT);
            }
            else if (__native_startup_state == __uninitialized)
            {
                __native_startup_state = __initializing;
#ifndef _SYSCRT
                if (_initterm_e( __xi_a, __xi_z ) != 0)
                {
                    return 255;
                }
#else  /* _SYSCRT */
                _initterm((_PVFV *)(void *)__xi_a, (_PVFV *)(void *)__xi_z);
#endif  /* _SYSCRT */
            }
            else
            {
                has_cctor = 1;
            }

            /*
            * do C++ constructors (initializers) specific to this EXE
            */
            if (__native_startup_state == __initializing)
            {
                _initterm( __xc_a, __xc_z );
                __native_startup_state = __initialized;
            }
            _ASSERTE(__native_startup_state == __initialized);
            if(!nested)
            {
                /* For X86, the definition of InterlockedExchangePointer wrongly causes warning C4312 */
#pragma warning(push)
#pragma warning(disable:4312)
                InterlockedExchangePointer((volatile PVOID *)&__native_startup_lock, 0);
#pragma warning(pop)
            }

            /*
             * If we have any dynamically initialized __declspec(thread)
             * variables, then invoke their initialization for the primary
             * thread used to start the process, by calling __dyn_tls_init
             * through a callback defined in tlsdyn.obj.
             */
            if (__dyn_tls_init_callback != NULL &&
                _IsNonwritableInCurrentImage((PBYTE)&__dyn_tls_init_callback))
            {
                __dyn_tls_init_callback(NULL, DLL_THREAD_ATTACH, NULL);
            }

            /* Enable buffer count checking if linking against static lib */
            _CrtSetCheckCount(TRUE);

#ifdef _WINMAIN_
            /*
             * Skip past program name (first token in command line).
             * Check for and handle quoted program name.
             */
#ifdef WPRFLAG
            /* OS may not support "W" flavors */
            if (_wcmdln == NULL)
                return 255;
            lpszCommandLine = (wchar_t *)_wcmdln;
#else  /* WPRFLAG */
            lpszCommandLine = (unsigned char *)_acmdln;
#endif  /* WPRFLAG */

            while (*lpszCommandLine > SPACECHAR ||
                   (*lpszCommandLine&&inDoubleQuote)) {
                /*
                 * Flip the count from 1 to 0 or 0 to 1 if current character
                 * is DOUBLEQUOTE
                 */
                if (*lpszCommandLine==DQUOTECHAR) inDoubleQuote=!inDoubleQuote;
#ifdef _MBCS
                if (_ismbblead(*lpszCommandLine)) {
                    if (lpszCommandLine) {
                        lpszCommandLine++;
                    }
                }
#endif  /* _MBCS */
                ++lpszCommandLine;
            }

            /*
             * Skip past any white space preceeding the second token.
             */
            while (*lpszCommandLine && (*lpszCommandLine <= SPACECHAR)) {
                lpszCommandLine++;
            }

#ifdef WPRFLAG
            mainret = wWinMain(
#else  /* WPRFLAG */
            mainret = WinMain(
#endif  /* WPRFLAG */
                       (HINSTANCE)&__ImageBase,
                       NULL,
                       lpszCommandLine,
                       StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                        ? StartupInfo.wShowWindow
                        : SW_SHOWDEFAULT
                      );
#else  /* _WINMAIN_ */

#ifdef WPRFLAG
            __winitenv = envp;
            mainret = wmain(argc, argv, envp);
#else  /* WPRFLAG */
            __initenv = envp;
            mainret = main(argc, argv, envp);
#endif  /* WPRFLAG */

#endif  /* _WINMAIN_ */

            /*
             * Note that if the exe is managed app, we don't really need to
             * call exit or _c_exit. .cctor should be able to take care of
             * this.
             */
            if ( !managedapp )
                exit(mainret);

            if (has_cctor == 0)
                _cexit();

        }
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
        {
            /*
             * Should never reach here
             */

            mainret = GetExceptionCode();

            /*
             * Note that if the exe is managed app, we don't really need to
             * call exit or _c_exit. .cctor should be able to take care of
             * this.
             */
            if ( !managedapp )
                _exit(mainret);

            if (has_cctor == 0)
                _cexit();
        } /* end of try - except */

        return mainret;
}

/***
*check_managed_app() - Check for a managed executable
*
*Purpose:
*       Determine if the EXE the startup code is linked into is a managed app
*       by looking for the COM Runtime Descriptor in the Image Data Directory
*       of the PE or PE+ header.
*
*Entry:
*       None
*
*Exit:
*       1 if managed app, 0 if not.
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl check_managed_app (
        void
        )
{
        PIMAGE_DOS_HEADER pDOSHeader;
        PIMAGE_NT_HEADERS pPEHeader;
        PIMAGE_OPTIONAL_HEADER32 pNTHeader32;
        PIMAGE_OPTIONAL_HEADER64 pNTHeader64;

        pDOSHeader = (PIMAGE_DOS_HEADER)&__ImageBase;
        if ( pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE )
            return 0;

        pPEHeader = (PIMAGE_NT_HEADERS)((char *)pDOSHeader +
                                        pDOSHeader->e_lfanew);
        if ( pPEHeader->Signature != IMAGE_NT_SIGNATURE )
            return 0;

        pNTHeader32 = (PIMAGE_OPTIONAL_HEADER32)&pPEHeader->OptionalHeader;
        switch ( pNTHeader32->Magic ) {
        case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
            /* PE header */
            /* prefast assumes we are overflowing __ImageBase */
#pragma warning(push)
#pragma warning(disable:26000)
            if ( pNTHeader32->NumberOfRvaAndSizes <=
                    IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR )
                return 0;
#pragma warning(pop)
            return !! pNTHeader32 ->
                      DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR] .
                      VirtualAddress;
        case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
            /* PE+ header */
            pNTHeader64 = (PIMAGE_OPTIONAL_HEADER64)pNTHeader32;
            if ( pNTHeader64->NumberOfRvaAndSizes <=
                    IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR )
                return 0;
            return !! pNTHeader64 ->
                      DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR] .
                      VirtualAddress;
        }

        /* Not PE or PE+, so not managed */
        return 0;
}

#endif  /* CRTDLL */
