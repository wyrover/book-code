/***
*crtlib.c - CRT DLL initialization and termination routine (Win32, Dosx32)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains initialization entry point for the CRT DLL
*       in the Win32 environment. It also contains some of the supporting
*       initialization and termination code.
*
*******************************************************************************/

#if defined (CRTDLL)

#include <cruntime.h>
#include <oscalls.h>
#include <dos.h>
#include <internal.h>
#include <malloc.h>
#include <mbctype.h>
#include <mtdll.h>
#include <process.h>
#include <rterr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <awint.h>
#include <tchar.h>
#include <time.h>
#include <io.h>
#include <dbgint.h>
#ifdef _SYSCRT
#include <ntverp.h>
#endif  /* _SYSCRT */
#include <setlocal.h>
#include <crtassem.h>

/*
 * This header is included for _getdrives(). This function
 * is used to take dependency on msvcrt.dll
 */
#include <direct.h>

/*
 * flag set iff _CRTDLL_INIT was called with DLL_PROCESS_ATTACH
 */
static int proc_attached = 0;

/*
 * command line, environment, and a few other globals
 */
wchar_t *_wcmdln = NULL;           /* points to wide command line */
char *_acmdln = NULL;              /* points to command line */

char *_aenvptr = NULL;      /* points to environment block */
wchar_t *_wenvptr = NULL;   /* points to wide environment block */

extern int _newmode;    /* declared in <internal.h> */

int __error_mode = _OUT_TO_DEFAULT;

int __app_type = _UNKNOWN_APP;

static void __cdecl inherit(void);  /* local function */

static BOOL __cdecl _check_manifest(HMODULE hDllHandle);  /* local function */

/***
*int __[w]getmainargs - get values for args to main()
*
*Purpose:
*       This function invokes the command line parsing and copies the args
*       to main back through the passsed pointers. The reason for doing
*       this here, rather than having _CRTDLL_INIT do the work and exporting
*       the __argc and __argv, is to support the linked-in option to have
*       wildcard characters in filename arguments expanded.
*
*Entry:
*       int *pargc              - pointer to argc
*       _TCHAR ***pargv         - pointer to argv
*       _TCHAR ***penvp         - pointer to envp
*       int dowildcard          - flag (true means expand wildcards in cmd line)
*       _startupinfo * startinfo- other info to be passed to CRT DLL
*
*Exit:
*       Returns 0 on success, negative if _*setargv returns an error. Values
*       for the arguments to main() are copied through the passed pointers.
*
*******************************************************************************/


_CRTIMP int __cdecl __wgetmainargs (
        int *pargc,
        wchar_t ***pargv,
        wchar_t ***penvp,
        int dowildcard,
        _startupinfo * startinfo)
{
        int ret;


        /* set global new mode flag */
        _newmode = startinfo->newmode;

        if ( dowildcard )
            ret = __wsetargv(); /* do wildcard expansion after parsing args */
        else
            ret = _wsetargv();  /* NO wildcard expansion; just parse args */
        if (ret < 0)
#ifdef _SYSCRT
            ExitProcess(-1);      // Failed to parse the cmdline - bail
#else  /* _SYSCRT */
            return ret;
#endif  /* _SYSCRT */

        *pargc = __argc;
        *pargv = __wargv;

        /*
         * if wide environment does not already exist,
         * create it from multibyte environment
         */
        if (!_wenviron)
        {
            _wenvptr=__crtGetEnvironmentStringsW();

            if (_wsetenvp() < 0)
            {
                __mbtow_environ();
            }
        }

        *penvp = _wenviron;

        return ret;
}



_CRTIMP int __cdecl __getmainargs (
        int *pargc,
        char ***pargv,
        char ***penvp,
        int dowildcard
        ,
        _startupinfo * startinfo
        )
{
        int ret;


        /* set global new mode flag */
        _newmode = startinfo->newmode;

        if ( dowildcard )
            ret = __setargv();  /* do wildcard expansion after parsing args */
        else
            ret = _setargv();   /* NO wildcard expansion; just parse args */
        if (ret < 0)
#ifdef _SYSCRT
            ExitProcess(-1);      // Failed to parse the cmdline - bail
#else  /* _SYSCRT */
            return ret;
#endif  /* _SYSCRT */

        *pargc = __argc;
        *pargv = __argv;
        *penvp = _environ;

        return ret;
}


/***
*BOOL _CRTDLL_INIT(hDllHandle, dwReason, lpreserved) - C DLL initialization.
*
*Purpose:
*       This routine does the C runtime initialization.
*
*Entry:
*
*Exit:
*
*******************************************************************************/

typedef void (__stdcall *NTVERSION_INFO_FCN)(PDWORD, PDWORD, PDWORD);

static
BOOL __cdecl
__CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        );

BOOL WINAPI
_CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        /*
         * The /GS security cookie must be initialized before any exception
         * handling targetting the current image is registered.  No function
         * using exception handling can be called in the current image until
         * after __security_init_cookie has been called.
         */
        __security_init_cookie();
    }

    return __CRTDLL_INIT(hDllHandle, dwReason, lpreserved);
}

__declspec(noinline)
BOOL __cdecl
__CRTDLL_INIT(
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved
        )
{
        unsigned int osplatform = 0;
        unsigned int winver = 0;
        unsigned int winmajor = 0;
        unsigned int winminor = 0;
        unsigned int osver = 0;

        if ( dwReason == DLL_PROCESS_ATTACH ) {
            OSVERSIONINFOA *posvi;
            /*
             * Dynamically allocate the OSVERSIONINFOA buffer, so we avoid
             * triggering the /GS buffer overrun detection.  That can't be
             * used here, since the guard cookie isn't available until we
             * initialize it from here!
             */
            posvi = (OSVERSIONINFOA *)HeapAlloc(GetProcessHeap(), 0, sizeof(OSVERSIONINFOA));
            if (!posvi)
                return FALSE;

            /*
             * Get the full Win32 version.
             */
            posvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
            if ( !GetVersionExA(posvi) ) {
                HeapFree(GetProcessHeap(), 0, posvi);
                return FALSE;
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
            posvi = NULL;
            if ( osplatform != VER_PLATFORM_WIN32_NT )
                osver |= 0x08000;
            winver = (winmajor << 8) + winminor;

            _set_osplatform(osplatform);
            _set_winver(winver);
            _set_winmajor(winmajor);
            _set_winminor(winminor);
            _set_osver(osver);

#if defined (_M_IX86) && defined (_SYSCRT)

            /*
             * Make sure we are NOT running on Win32s
             */
            if ( osplatform == VER_PLATFORM_WIN32s ) {
                __crtMessageBoxA("MSVCRT.DLL for Win32\n\nError: MSVCRT.DLL is not compatible with Win32s.",
                         "Microsoft Visual C++ Runtime Library",
                         MB_OK|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);
                return FALSE;
            }

#endif  /* defined (_M_IX86) && defined (_SYSCRT) */

            if ( !_heap_init(1) )   /* initialize heap */
                /*
                 * The heap cannot be initialized, return failure to the
                 * loader.
                 */
                return FALSE;

            if(!_mtinit())          /* initialize multi-thread */
            {
                /*
                 * If the DLL load is going to fail, we must clean up
                 * all resources that have already been allocated.
                 */
                _heap_term();       /* heap is now invalid! */

                return FALSE;       /* fail DLL load on failure */
            }

            if (_ioinit() < 0) {    /* inherit file info */
                /* Clean up already-allocated resources */
                /* free TLS index, call _mtdeletelocks() */
                _mtterm();

                _heap_term();       /* heap is now invalid! */

                return FALSE;       /* fail DLL load on failure */
            }

            _aenvptr = (char *)__crtGetEnvironmentStringsA();

            _acmdln = (char *)GetCommandLineA();
            _wcmdln = (wchar_t *)__crtGetCommandLineW();

#ifdef _MBCS
            /*
             * Initialize multibyte ctype table. Always done since it is
             * needed for processing the environment strings.
             */
            __initmbctable();
#endif  /* _MBCS */

            /*
             * For CRT DLL, since we don't know the type (wide or multibyte)
             * of the program, we create only the multibyte type since that
             * is by far the most likely case. Wide environment will be created
             * on demand as usual.
             */

            if (_setenvp() < 0 ||   /* get environ info */
                _cinit(FALSE) != 0)  /* do C data initialize */
            {
                _ioterm();          /* shut down lowio */
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
                _heap_term();       /* heap is now invalid! */
                return FALSE;       /* fail DLL load on failure */
            }

#if defined (_CRT_CHECK_MANIFEST)
            if (!_check_manifest(hDllHandle))
            {
                __try
                {
                    _NMSG_WRITE(_RT_CHECKMANIFEST);
                }
                __except( EXCEPTION_EXECUTE_HANDLER )
                {
                    OutputDebugString(_GET_RTERRMSG(_RT_CHECKMANIFEST));
                    DebugBreak();
                }

                _ioterm();          /* shut down lowio */
                _mtterm();          /* free TLS index, call _mtdeletelocks() */
                _heap_term();       /* heap is now invalid! */
                return FALSE;
            }
#endif  /* defined (_CRT_CHECK_MANIFEST) */

            /*
             * Increment flag indicating process attach notification
             * has been received.
             */
            proc_attached++;

        }
        else if ( dwReason == DLL_PROCESS_DETACH ) {
            /*
             * if a client process is detaching, make sure minimal
             * runtime termination is performed and clean up our
             * 'locks' (i.e., delete critical sections).
             */
            if ( proc_attached > 0 ) {
                proc_attached--;

                /*
                 * Any basic clean-up done here may also need
                 * to be done below if Process Attach is partly
                 * processed and then a failure is encountered.
                 */

                if ( _C_Termination_Done == FALSE )
                    _cexit();

                __crtdll_callstaticterminators();

#ifdef _DEBUG
                /* Dump all memory leaks */
                if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_LEAK_CHECK_DF)
                {
                    _CrtSetDumpClient(NULL);
                    __freeCrtMemory();
                    _CrtDumpMemoryLeaks();
                }
#endif  /* _DEBUG */

                /*
                 * What remains is to clean up the system resources we have
                 * used (handles, critical sections, memory,...,etc.). This
                 * needs to be done if the whole process is NOT terminating.
                 */
                if ( lpreserved == NULL )
                {
                    /*
                     * The process is NOT terminating so we must clean up...
                     */
                    _ioterm();
                    /* free TLS index, call _mtdeletelocks() */
                    _mtterm();

                    /* This should be the last thing the C run-time does */
                    _heap_term();   /* heap is now invalid! */
                }

            }
            else
                /* no prior process attach, just return */
                return FALSE;

        }
        else if ( dwReason == DLL_THREAD_ATTACH )
        {
            _ptiddata ptd;

            /* Initialize FlsGetValue function pointer */
            __set_flsgetvalue();

            if ( (ptd = FLS_GETVALUE(__flsindex)) == NULL)
            {
                if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) != NULL))
                {
                    if (FLS_SETVALUE(__flsindex, (LPVOID)ptd) ) {
                        /*
                        * Initialize of per-thread data
                        */
                        _initptd(ptd, NULL);

                        ptd->_tid = GetCurrentThreadId();
                        ptd->_thandle = (uintptr_t)(-1);
                    } else
                    {
                        _free_crt(ptd);
                        return FALSE;
                    }
                } else
                {
                    return FALSE;
                }
            }
        }
        else if ( dwReason == DLL_THREAD_DETACH )
        {
            _freeptd(NULL);     /* free up per-thread CRT data */
        }
        else
        {
            /*
             * Note that we are using this to add dependency to msvcrt.dll.
             * Also the control should never go through this block as we have
             * all the possible values for dwReason in else if blocks.
             *
             * The reson we need _getdrives in some function that is called from
             * either export table or from some other function is that reference
             * to _getdrives in msvcrt.dll should not be removed because of
             * /OPT:REF.
             */
            _getdrives();
        }


        return TRUE;
}

/* used in _enforce_manifest */
#define _PATH_LEN 8000
#define _WINSXS_FOLDER L"WinSxS\\"
#define _MSCORWKS_DLL_FILENAME L"mscorwks.dll"

#ifdef _DEBUG
#define _MANIFEST_FILENAME _CRT_WIDE(__LIBRARIES_ASSEMBLY_NAME_PREFIX) L".DebugCRT.manifest"
#define _CRT_DLL_FILENAME L"msvcr80d.dll"
#else  /* _DEBUG */
#define _MANIFEST_FILENAME _CRT_WIDE(__LIBRARIES_ASSEMBLY_NAME_PREFIX) L".CRT.manifest"
#define _CRT_DLL_FILENAME L"msvcr80.dll"
#endif  /* _DEBUG */

typedef BOOL (WINAPI * PFN_FINDAC)(DWORD dwFlags, const GUID *lpExtensionGuid,ULONG ulSectionId,LPCWSTR lpStringToFind,PACTCTX_SECTION_KEYED_DATA ReturnedData);
typedef UINT (WINAPI * PFN_GETSYSTEMWINDOWSDIRECTORY)(LPWSTR lpBuffer,  UINT uSize);


static BOOL __cdecl _check_manifest(HMODULE hDllHandle)
{
    /* We check that the dll is loaded through a manifest.
     *
     * We check several conditions and exceptions:
     *
     * (1)  if (pre-fusion OS)
     *          return TRUE; [no need to check]
     *
     * (2)  if dll is being loaded by instrumented-mscoree.dll.
     *          return TRUE;
     *
     * (3)  if (dll is loaded from system32)
     *          return FALSE;
     *
     * (4)  if (!(loaded through a manifest))
     *          return FALSE;
     *
     * (5)  if (loaded from %SystemRoot%\WinSxS)
     *          return TRUE; [loaded from the WinSxS cache]
     *
     * (6)  if (manifest is in the same folder as the dll)
     *          return TRUE;
     *
     * (7)  return FALSE; [loaded with another manifest]
     *
     * In general, when we encounter an error condition or something
     * which blocks us from checking if the dll is loaded through a
     * manifest, we do not return an error, but we let the dll to be
     * loaded. Notice that this is not a security feature: it's an helper
     * which will try (as much as possible) to discourage the practice
     * of not using a manifest to load the crt dll.
     */

    wchar_t moduleFilePath[_PATH_LEN];
    wchar_t systemRoot[MAX_PATH];
    size_t moduleFilePathLen = 0;
    size_t systemRootLen = 0;
    PFN_FINDAC pfnFindActCtxSectionStringW =NULL;

    /* check condition (1) */
    {
        HINSTANCE hKernel32 = GetModuleHandle("kernel32.dll");
        if (hKernel32 == NULL)
        {
            /* unexpected condition, probably some strange loader lock situation */
            return TRUE;
        }

        pfnFindActCtxSectionStringW = (PFN_FINDAC) GetProcAddress(hKernel32, "FindActCtxSectionStringW");

        if (pfnFindActCtxSectionStringW == NULL)
        {
            /* pre-fusion OS, so no more checking.*/
            return TRUE;
        }
    }

    /* being loaded by instrumented-mscoree.dll. (2) */
    {
        HINSTANCE hMSCoree = GetModuleHandle("MSCoree.dll");
        HINSTANCE hPGORT80 = GetModuleHandle("PGORT80.dll");
        if( hMSCoree && hPGORT80 )
        {
            // If pogo-instrumented mscoree is loaded, it has
            // a dependency on pgort80.dll. However, the manifest
            // in pgort80.dll is ignored.
            return TRUE;
        }
    }

    /* retrieve moduleFilePath and systemRoot */
    {
        wchar_t tempPath[_PATH_LEN];
        wchar_t *moduleName = NULL;
        DWORD ret = GetModuleFileNameW(hDllHandle, tempPath, _countof(tempPath));
        if (ret == 0 || ret >= _countof(tempPath))
        {
            /* error or filename longer than _PATH_LEN: we just let
             * the user load the dll, without checking the manifest */
            return TRUE;
        }
        ret = GetLongPathNameW(tempPath, moduleFilePath, _countof(moduleFilePath));
        if (ret == 0 || ret >= _countof(moduleFilePath))
        {
            /* error or filename longer than _PATH_LEN: we just let
             * the user load the dll, without checking the manifest */
            return TRUE;
        }

        moduleName = wcsrchr(moduleFilePath, L'\\');
        if (moduleName == NULL)
        {
            /* error: we could not find a backslash in moduleFilePath */
            return TRUE;
        }
        moduleName++;
        *moduleName = 0;
        /* moduleFilePath now contains only the path of the module ending with a backslash */
        moduleFilePathLen = wcslen(moduleFilePath);

        ret = GetSystemDirectoryW(systemRoot, _countof(systemRoot));
        if (ret == 0 || ret >= _countof(systemRoot))
        {
            /* error or path longer than MAX_PATH: we just let
             * the user load the dll, without checking the manifest */
            return TRUE;
        }
        /* add a trailing backslash if not there already */
        systemRootLen = ret;
        if (systemRoot[systemRootLen - 1] != L'\\')
        {
            if (wcsncat_s(systemRoot, _countof(systemRoot), L"\\", _TRUNCATE) != 0)
            {
                /* error or not enough space */
                return TRUE;
            }
        }
        systemRootLen = wcslen(systemRoot);
    }

    /* check condition (3) */
    if (moduleFilePathLen == systemRootLen &&
        _wcsicmp(moduleFilePath, systemRoot) == 0)
    {
        return FALSE; /* dll loaded from system32 */
    }

    /* check condition (4) */
    {
        ACTCTX_SECTION_KEYED_DATA askd = { sizeof(askd) };
        if (!(*pfnFindActCtxSectionStringW)(0, NULL, ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION, _CRT_DLL_FILENAME, &askd))
        {
                        /* no activation context used to load CRT DLL, means no manifest present in the process */
                        return FALSE;
                }
    }

    /* check condition (5) */
    {
        HINSTANCE hKernel32 = GetModuleHandle("kernel32.dll");
        PFN_GETSYSTEMWINDOWSDIRECTORY pfnGetSystemWindowsDirectory = NULL;
        if (hKernel32 == NULL)
        {
            /* unexpected condition, probably some strange loader lock situation */
            return TRUE;
        }

        pfnGetSystemWindowsDirectory = (PFN_GETSYSTEMWINDOWSDIRECTORY) GetProcAddress(hKernel32, "GetSystemWindowsDirectoryW");

        if (pfnGetSystemWindowsDirectory == NULL)
        {
            /* pre-win2k OS, so no more checking. We shouldn't get to here because we checked OS version earlier */
            return TRUE;
        }

        /* retrieves the windows folder */
        systemRootLen = pfnGetSystemWindowsDirectory(systemRoot, _countof(systemRoot));
        if (systemRootLen == 0 || systemRootLen >= _countof(systemRoot))
        {
            /* error or path longer than MAX_PATH: we just let
             * the user load the dll, without checking the manifest */
            return TRUE;
        }
        /* add a trailing backslash if not there already */
        if (systemRoot[systemRootLen - 1] != L'\\')
        {
            if (wcsncat_s(systemRoot, _countof(systemRoot), L"\\" _WINSXS_FOLDER, _TRUNCATE) != 0)
            {
                /* error or not enough space */
                return TRUE;
            }
        }
        else
        {
            if (wcsncat_s(systemRoot, _countof(systemRoot), _WINSXS_FOLDER, _TRUNCATE) != 0)
            {
                /* error or not enough space */
                return TRUE;
            }
        }
        systemRootLen = wcslen(systemRoot);

        if (_wcsnicmp(systemRoot, moduleFilePath, systemRootLen) == 0)
        {
            /* moduleFileName begins with %SYSTEMROOT%\WinSxS\ */
            return TRUE; /* dll has been loaded from the WinSxS cache */
        }
    }

    /* check condition (6) */
    {
        struct _wfinddata_t findData = { 0 };
        intptr_t findHandle = -1;

        if (wcsncat_s(moduleFilePath, _countof(moduleFilePath), _MANIFEST_FILENAME, _TRUNCATE) != 0)
        {
            /* error or truncation */
            return TRUE;
        }
        findHandle = _wfindfirst(moduleFilePath, &findData);
        if (findHandle != -1)
        {
            _findclose(findHandle);
            return TRUE; /* manifest is in the same folder as the dll */
        }
        /* we could not find the manifest in the same folder as the dll */
        moduleFilePath[moduleFilePathLen] = 0;
        /* restore moduleFilePath so that it contains only the path of the module with final backslash */
    }

    /* case (7): loaded with another manifest */
    return FALSE;
}

/*
 * Earlier the below exports were only for X86, but with invent of /clr:pure,
 * they need to be defined for all the platform.
 */

/*
 * Functions to access user-visible, per-process variables
 */

/*
 * Macro to construct the name of the access function from the variable
 * name.
 */
#define AFNAME(var) __p_ ## var

/*
 * Macro to construct the access function's return value from the variable
 * name.
 */
#define AFRET(var)  &var

/*
 ***
 ***  Template
 ***

_CRTIMP __cdecl
AFNAME() (void)
{
        return AFRET();
}

 ***
 ***
 ***
 */

#ifdef _DEBUG

_CRTIMP long *
AFNAME(_crtAssertBusy) (void)
{
        return AFRET(_crtAssertBusy);
}

_CRTIMP long *
AFNAME(_crtBreakAlloc) (void)
{
        return AFRET(_crtBreakAlloc);
}

_CRTIMP int *
AFNAME(_crtDbgFlag) (void)
{
        return AFRET(_crtDbgFlag);
}

#endif  /* _DEBUG */

_CRTIMP char ** __cdecl
AFNAME(_acmdln) (void)
{
        return AFRET(_acmdln);
}

_CRTIMP wchar_t ** __cdecl
AFNAME(_wcmdln) (void)
{
        return AFRET(_wcmdln);
}

_CRTIMP unsigned int * __cdecl
AFNAME(_amblksiz) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_amblksiz);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP int * __cdecl
AFNAME(__argc) (void)
{
        return AFRET(__argc);
}

_CRTIMP char *** __cdecl
AFNAME(__argv) (void)
{
        return AFRET(__argv);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(__wargv) (void)
{
        return AFRET(__wargv);
}

_CRTIMP int * __cdecl
AFNAME(_commode) (void)
{
        return AFRET(_commode);
}

_CRTIMP int * __cdecl
AFNAME(_daylight) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_daylight);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP long * __cdecl
AFNAME(_dstbias) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_dstbias);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP char *** __cdecl
AFNAME(_environ) (void)
{
        return AFRET(_environ);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(_wenviron) (void)
{
        return AFRET(_wenviron);
}

_CRTIMP int * __cdecl
AFNAME(_fmode) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_fmode);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP char *** __cdecl
AFNAME(__initenv) (void)
{
        return AFRET(__initenv);
}

_CRTIMP wchar_t *** __cdecl
AFNAME(__winitenv) (void)
{
        return AFRET(__winitenv);
}

_CRTIMP FILE *
AFNAME(_iob) (void)
{
        return &_iob[0];
}

_CRTIMP unsigned char * __cdecl
AFNAME(_mbctype) (void)
{
        return &_mbctype[0];
}

_CRTIMP unsigned char * __cdecl
AFNAME(_mbcasemap) (void)
{
        return &_mbcasemap[0];
}

_CRTIMP int * __cdecl
AFNAME(__mb_cur_max) (void)
{
        _ptiddata ptd = _getptd();
        pthreadlocinfo ptloci = ptd->ptlocinfo;

        __UPDATE_LOCALE(ptd, ptloci);
        return AFRET(__MB_CUR_MAX(ptloci));
}


_CRTIMP unsigned int * __cdecl
AFNAME(_osver) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_osver);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP unsigned int * __cdecl
AFNAME(_osplatform) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_osplatform);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP const unsigned short ** __cdecl
AFNAME(_pctype) (void)
{
        _ptiddata ptd = _getptd();
        pthreadlocinfo ptloci = ptd->ptlocinfo;

        __UPDATE_LOCALE(ptd, ptloci);
        return AFRET(ptloci->pctype);
}

_CRTIMP const unsigned short ** __cdecl
AFNAME(_pwctype) (void)
{
        return AFRET(_pwctype);
}

_CRTIMP char **  __cdecl
AFNAME(_pgmptr) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_pgmptr);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP wchar_t ** __cdecl
AFNAME(_wpgmptr) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_wpgmptr);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP long * __cdecl
AFNAME(_timezone) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_timezone);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP char ** __cdecl
AFNAME(_tzname) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return &_tzname[0];
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winmajor) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_winmajor);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winminor) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_winminor);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

_CRTIMP unsigned int * __cdecl
AFNAME(_winver) (void)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        return AFRET(_winver);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

#endif  /* defined (CRTDLL) */
