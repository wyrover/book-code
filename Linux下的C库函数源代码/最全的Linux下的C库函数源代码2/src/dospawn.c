/***
*dospawn.c - spawn a child process
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _dospawn - spawn a child process
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <msdos.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <tchar.h>
#include <dbgint.h>
#include <limits.h>

#ifndef WPRFLAG
int _p_overlay = 2;
#endif  /* WPRFLAG */

/***
*int _dospawn(mode, name, cmdblk, envblk) - spawn a child process
*
*Purpose:
*       Spawns a child process
*
*Entry:
*       int mode     - _P_WAIT, _P_NOWAIT, _P_NOWAITO, _P_OVERLAY, or _P_DETACH
*       _TSCHAR *name   - name of program to execute
*       _TSCHAR *cmdblk - parameter block
*       _TSCHAR *envblk - environment block
*
*Exit:
*       _P_OVERLAY: -1 = error, otherwise doesn't return
*       _P_WAIT:    termination code << 8 + result code
*       _P_DETACH: -1 = error, 0 = success
*       others:    PID of process
*
*Exceptions:
*
*******************************************************************************/

#ifdef WPRFLAG
intptr_t __cdecl _wdospawn (
#else  /* WPRFLAG */
intptr_t __cdecl _dospawn (
#endif  /* WPRFLAG */
        int mode,
        const _TSCHAR *name,
        _TSCHAR *cmdblk,
        _TSCHAR *envblk
        )
{
        char syncexec, asyncresult, background;
        LPTSTR CommandLine;
        STARTUPINFO StartupInfo;
        PROCESS_INFORMATION ProcessInformation;
        BOOL CreateProcessStatus;
        ULONG dosretval;                /* OS return value */
        DWORD exitcode;
        intptr_t retval;
        DWORD fdwCreate = 0;            /* flags for CreateProcess */
        int i;
        ioinfo *pio;
        char *posfile;
        UNALIGNED intptr_t *posfhnd;
        int nh;                         /* number of file handles to be
                                           passed to the child */

        /* translate input mode value to individual flags */
        syncexec = asyncresult = background = 0;
        switch (mode) {
        case _P_WAIT:    syncexec=1;    break;  /* synchronous execution */
        case 2: /* _P_OVERLAY */
        case _P_NOWAITO: break;                 /* asynchronous execution */
        case _P_NOWAIT:  asyncresult=1; break;  /* asynch + remember result */
        case _P_DETACH:  background=1;  break;  /* detached in null scrn grp */
        default:
            _doserrno = 0;              /* not a Dos error */
            _VALIDATE_RETURN(("invalid mode", 0), EINVAL, -1);
        }

        /*
         * Loop over null separate arguments, and replace null separators
         * with spaces to turn it back into a single null terminated
         * command line.
         */
        CommandLine = cmdblk;
        while (*cmdblk) {
            while (*cmdblk) {
                cmdblk++;
            }

            /*
             * If not last argument, turn null separator into a space.
             */
            if (cmdblk[1] != _T('\0')) {
                *cmdblk++ = _T(' ');
            }
        }

        memset(&StartupInfo,0,sizeof(StartupInfo));
        StartupInfo.cb = sizeof(StartupInfo);

        for ( nh = _nhandle ;
              nh && !_osfile(nh - 1) ;
              nh-- ) ;

        if (nh < 0 ||
            nh >= ((USHRT_MAX - sizeof(int)) / (sizeof(char) + sizeof(intptr_t))))
        {
            errno = ENOMEM;
            return -1;
        }

        StartupInfo.cbReserved2 =
            (unsigned short)(sizeof(int) + (nh * (sizeof(char) + sizeof(intptr_t))));

        StartupInfo.lpReserved2 = _calloc_crt( StartupInfo.cbReserved2, 1 );

        if (!StartupInfo.lpReserved2)
        {
            errno = ENOMEM;
            return -1;
        }

        *((UNALIGNED int *)(StartupInfo.lpReserved2)) = nh;

        posfile = (char *)(StartupInfo.lpReserved2 + sizeof(int));
        posfhnd = (UNALIGNED intptr_t *)(StartupInfo.lpReserved2 +
            sizeof(int) + (nh * sizeof(char)));

        for ( i = 0;
#pragma warning(suppress: 22009) /* Here prefast is very confused by this nh */
              i < nh ;
              i++, posfile++, posfhnd++ )
        {
            pio = _pioinfo(i);
            if ( (pio->osfile & FNOINHERIT) == 0 ) {
                *posfile = pio->osfile;
                *posfhnd = pio->osfhnd;
            }
            else {
                *posfile = 0;
                *posfhnd = (intptr_t)INVALID_HANDLE_VALUE;
            }
        }

        /*
         * if the child process is detached, it cannot access the console, so
         * we must nuke the information passed for the first three handles.
         */
        if ( background ) {

            for ( i = 0,
                  posfile = (char *)(StartupInfo.lpReserved2 + sizeof( int )),
                  posfhnd = (UNALIGNED intptr_t *)(StartupInfo.lpReserved2 + sizeof( int )
                            + (nh * sizeof( char ))) ;
                  i < __min( nh, 3 ) ;
                  i++, posfile++, posfhnd++ )
            {
                *posfile = 0;
                *posfhnd = (intptr_t)INVALID_HANDLE_VALUE;
            }

            fdwCreate |= DETACHED_PROCESS;
        }

        /*
         * errno is set to something non-zero if there are some
         * errors in the spawning
         */
        _doserrno = 0;

#ifdef WPRFLAG
        /* indicate to CreateProcess that environment block is wide */
        fdwCreate |= CREATE_UNICODE_ENVIRONMENT;
#endif  /* WPRFLAG */

        CreateProcessStatus = CreateProcess( (LPTSTR)name,
                                             CommandLine,
                                             NULL,
                                             NULL,
                                             TRUE,
                                             fdwCreate,
                                             envblk,
                                             NULL,
                                             &StartupInfo,
                                             &ProcessInformation
                                           );

        dosretval = GetLastError();
        _free_crt( StartupInfo.lpReserved2 );

        if (!CreateProcessStatus) {
            _dosmaperr(dosretval);
            return -1;
        }

        if (mode == 2 /* _P_OVERLAY */) {
            /* destroy ourselves */
            _exit(0);
        }
        else if (mode == _P_WAIT) {
            WaitForSingleObject(ProcessInformation.hProcess, (DWORD)(-1L));

            /* return termination code and exit code -- note we return
               the full exit code */
            GetExitCodeProcess(ProcessInformation.hProcess, &exitcode);

            retval = (intptr_t)(int)exitcode;

            CloseHandle(ProcessInformation.hProcess);
        }
        else if (mode == _P_DETACH) {
            /* like totally detached asynchronous spawn, dude,
               close process handle, return 0 for success */
            CloseHandle(ProcessInformation.hProcess);
            retval = (intptr_t)0;
        }
        else {
            /* asynchronous spawn -- return PID */
            retval = (intptr_t)ProcessInformation.hProcess;
        }

        CloseHandle(ProcessInformation.hThread);
        return retval;
}
