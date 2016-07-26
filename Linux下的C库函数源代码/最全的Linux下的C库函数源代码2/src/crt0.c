/***
*crt0.c - C runtime initialization routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This is the actual startup routine for apps.  It calls the user's main
*       routine [w]main() or [w]WinMain after performing C Run-Time Library
*       initialization.
*
*       With ifdefs, this source file also provides the source code for:
*       wcrt0.c     the startup routine for console apps with wide chars
*       wincrt0.c   the startup routine for Windows apps
*       wwincrt0.c  the startup routine for Windows apps with wide chars
*
*******************************************************************************/

#ifndef CRTDLL

#include <cruntime.h>
#include <dos.h>
#include <internal.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>
#include <rterr.h>
#include <rtcapi.h>
#include <windows.h>
#include <awint.h>
#include <tchar.h>
#include <dbgint.h>
#include <locale.h>

/*
 * wWinMain is not yet defined in winbase.h. When it is, this should be
 * removed.
 */

#if defined (_WIN64) && defined (_M_IA64)
#pragma section(".base", long, read)
__declspec(allocate(".base"))
extern
IMAGE_DOS_HEADER __ImageBase;
#else  /* defined (_WIN64) && defined (_M_IA64) */
extern
IMAGE_DOS_HEADER __ImageBase;
#endif  /* defined (_WIN64) && defined (_M_IA64) */

int
WINAPI
wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nShowCmd
    );

#ifdef WPRFLAG
_TUCHAR * __cdecl _wwincmdln(void);
#else  /* WPRFLAG */
_TUCHAR * __cdecl _wincmdln(void);
#endif  /* WPRFLAG */

/*
 * command line, environment, and a few other globals
 */

#ifdef WPRFLAG
wchar_t *_wcmdln;           /* points to wide command line */
#else  /* WPRFLAG */
char *_acmdln;              /* points to command line */
#endif  /* WPRFLAG */

char *_aenvptr = NULL;      /* points to environment block */
wchar_t *_wenvptr = NULL;   /* points to wide environment block */


static void __cdecl fast_error_exit(int);   /* Error exit via ExitProcess */

static int __cdecl check_managed_app(void); /* Determine if a managed app */

/*
 * _error_mode and _apptype, together, determine how error messages are
 * written out.
 */
int __error_mode = _OUT_TO_DEFAULT;
#ifdef _WINMAIN_
int __app_type = _GUI_APP;
#else  /* _WINMAIN_ */
int __app_type = _CONSOLE_APP;
#endif  /* _WINMAIN_ */


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

#ifdef _WINMAIN_

#ifdef WPRFLAG
#define _tmainCRTStartup    wWinMainCRTStartup
#else  /* WPRFLAG */
#define _tmainCRTStartup    WinMainCRTStartup
#endif  /* WPRFLAG */

#else  /* _WINMAIN_ */

#ifdef WPRFLAG
#define _tmainCRTStartup    wmainCRTStartup
#else  /* WPRFLAG */
#define _tmainCRTStartup    mainCRTStartup
#endif  /* WPRFLAG */

#endif  /* _WINMAIN_ */

#ifdef WPRFLAG

#define _twincmdln  _wwincmdln
#define _tinitenv   __winitenv
#define _targv      __wargv
#define _tcmdln     _wcmdln
#define _tenvptr    _wenvptr
#define _tsetargv   _wsetargv
#define _tsetenvp   _wsetenvp
#define GetCommandLineT         __crtGetCommandLineW
#define GetEnvironmentStringsT  __crtGetEnvironmentStringsW

#else  /* WPRFLAG */

#define _twincmdln  _wincmdln
#define _tinitenv   __initenv
#define _targv      __argv
#define _tcmdln     _acmdln
#define _tenvptr    _aenvptr
#define _tsetargv   _setargv
#define _tsetenvp   _setenvp
#define GetCommandLineT         GetCommandLineA
#define GetEnvironmentStringsT  __crtGetEnvironmentStringsA

#endif  /* WPRFLAG */

static
int
__tmainCRTStartup(
         void
         );

int
_tmainCRTStartup(
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
        unsigned int osplatform = 0;
        unsigned int winver = 0;
        unsigned int winmajor = 0;
        unsigned int winminor = 0;
        unsigned int osver = 0;
        int initret;
        int mainret=0;
        OSVERSIONINFOA *posvi;
        int managedapp;
#ifdef _WINMAIN_
        _TUCHAR *lpszCommandLine;
        STARTUPINFO StartupInfo;

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
         * Dynamically allocate the OSVERSIONINFOA buffer, so we avoid
         * triggering the /GS buffer overrun detection.  That can't be
         * used here, since the guard cookie isn't available until we
         * initialize it from here!
         */
        posvi = (OSVERSIONINFOA *)HeapAlloc(GetProcessHeap(), 0, sizeof(OSVERSIONINFOA));
        if (!posvi) {
            fast_error_exit(_RT_HEAP);
            return 255;
        }

        /*
         * Get the full Win32 version
         */
        posvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
        if ( !GetVersionExA(posvi) ) {
            HeapFree(GetProcessHeap(), 0, posvi);
            return 255;
        }

        osplatform = posvi->dwPlatformId;
        winmajor = posvi->dwMajorVersion;
        winminor = posvi->dwMinorVersion;

        /*
         * The somewhat bizarre calculations of _osver and _winver are
         * required for backward compatibility (used to use GetVersion)
         */
        osver = (posvi->dwBuildNumber) & 0x07fff;
        HeapFree(GetProcessHeap(), 0, posvi);
        if ( osplatform != VER_PLATFORM_WIN32_NT )
            osver |= 0x08000;
        winver = (winmajor << 8) + winminor;

        _set_osplatform(osplatform);
        _set_winver(winver);
        _set_winmajor(winmajor);
        _set_winminor(winminor);
        _set_osver(osver);

        /*
         * Determine if this is a managed application
         */
        managedapp = check_managed_app();

        if ( !_heap_init(1) )               /* initialize heap */
            fast_error_exit(_RT_HEAPINIT);  /* write message and die */

        if( !_mtinit() )                    /* initialize multi-thread */
            fast_error_exit(_RT_THREAD);    /* write message and die */

        /* Enable buffer count checking if linking against static lib */
        _CrtSetCheckCount(TRUE);

        /*
         * Initialize the Runtime Checks stuff
         */
#ifdef _RTC
        _RTC_Initialize();
#endif  /* _RTC */
        /*
         * Guard the remainder of the initialization code and the call
         * to user's main, or WinMain, function in a __try/__except
         * statement.
         */

        __try {

            if ( _ioinit() < 0 )            /* initialize lowio */
                _amsg_exit(_RT_LOWIOINIT);

            /* get wide cmd line info */
            _tcmdln = (_TSCHAR *)GetCommandLineT();

            /* get wide environ info */
            _tenvptr = (_TSCHAR *)GetEnvironmentStringsT();

            if ( _tsetargv() < 0 )
                _amsg_exit(_RT_SPACEARG);
            if ( _tsetenvp() < 0 )
                _amsg_exit(_RT_SPACEENV);

            initret = _cinit(TRUE);                  /* do C data initialize */
            if (initret != 0)
                _amsg_exit(initret);

#ifdef _WINMAIN_


            lpszCommandLine = _twincmdln();
            mainret = _tWinMain( (HINSTANCE)&__ImageBase,
                                 NULL,
                                 lpszCommandLine,
                                 StartupInfo.dwFlags & STARTF_USESHOWWINDOW
                                      ? StartupInfo.wShowWindow
                                      : SW_SHOWDEFAULT
                                );
#else  /* _WINMAIN_ */
            _tinitenv = _tenviron;
            mainret = _tmain(__argc, _targv, _tenviron);
#endif  /* _WINMAIN_ */

            if ( !managedapp )
                exit(mainret);

            _cexit();

        }
        __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
        {
            /*
             * Should never reach here
             */

            mainret = GetExceptionCode();

            if ( !managedapp )
                _exit(mainret);

            _c_exit();

        } /* end of try - except */

        return mainret;
}


/***
*fast_error_exit(rterrnum) - Faster exit fatal errors
*
*Purpose:
*       Exit the process with error code of 255 and appropriate error
*       message.
*
*Entry:
*       int rterrnum - error message number (amsg_exit only).
*
*Exit:
*       Calls ExitProcess (through __crtExitProcess).
*
*Exceptions:
*
*******************************************************************************/

static void __cdecl fast_error_exit (
        int rterrnum
        )
{
    /*
     * Note that here there is no other option other then to use __error_mode
     * as even if we use _set_error_mode, there is very slim possiblity if
     * proper _ser_error_mode forwarding is really working.
     */
#ifdef _WINMAIN_
        if ( __error_mode == _OUT_TO_STDERR )
#else  /* _WINMAIN_ */
        if ( __error_mode != _OUT_TO_MSGBOX )
#endif  /* _WINMAIN_ */
            _FF_MSGBANNER();    /* write run-time error banner */

        _NMSG_WRITE(rterrnum);  /* write message */
        __crtExitProcess(255);  /* normally _exit(255) */
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

        pDOSHeader = &__ImageBase;
        if ( pDOSHeader == NULL || pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE )
            return 0;

        pPEHeader = (PIMAGE_NT_HEADERS)((char *)pDOSHeader +
                                        pDOSHeader->e_lfanew);

        if ( pPEHeader->Signature != IMAGE_NT_SIGNATURE )
            return 0;

        if (pPEHeader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC)
            return 0;

        /* prefast assumes we are overflowing __ImageBase */
#pragma warning(push)
#pragma warning(disable:26000)
        if (pPEHeader->OptionalHeader.NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR)
            return 0;
#pragma warning(push)

        return pPEHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress != 0;
}

#ifndef WPRFLAG


#endif  /* WPRFLAG */

#endif  /* CRTDLL */
