/***
*cenvarg.c - set up environment, command line blocks
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _cenvarg() - setup environment/command line blocks
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <errno.h>
#include <msdos.h>
#include <stdlib.h>
#include <stdarg.h>
#include <internal.h>
#include <string.h>
#include <awint.h>
#include <tchar.h>
#include <dbgint.h>

/* local tchar */
#ifdef WPRFLAG
#define _tenvptr    _wenvptr
#else  /* WPRFLAG */
#define _tenvptr    _aenvptr
#endif  /* WPRFLAG */

/***
*int _cenvarg(argv, envp, argblk, envblk, name) - set up cmd line/environ
*
*Purpose:
*       Set up the block forms of  the environment and the command line.
*       If "envp" is null, "_environ" is used instead.
*
*Entry:
*       _TSCHAR **argv   - argument vector
*       _TSCHAR **envp   - environment vector
*       _TSCHAR **argblk - pointer to pointer set to malloc'ed space for args
*       _TSCHAR **envblk - pointer to pointer set to malloc'ed space for env
*       _TSCHAR *name    - name of program being invoked
*
*Exit:
*       returns 0 if ok, -1 if fails
*       stores through argblk and envblk
*       (calls malloc)
*
*Exceptions:
*
*******************************************************************************/

#ifdef WPRFLAG
int __cdecl _wcenvarg (
#else  /* WPRFLAG */
int __cdecl _cenvarg (
#endif  /* WPRFLAG */
        const _TSCHAR * const *argv,
        const _TSCHAR * const *envp,
        _TSCHAR **argblk,
        _TSCHAR **envblk,
        const _TSCHAR *name
        )
{
        REG1 const _TSCHAR * const *vp;
        REG2 unsigned tmp;
        REG3 _TSCHAR *cptr;
        unsigned arg_len;
        unsigned env_len;
        int cfi_len;            /* counts the number of file handles in CFI */
        int retval = 0;

        /*
         * Null environment pointer "envp" means use global variable,
         * "_environ"
         */

        int cwd_start;
        int cwd_end;            /* length of "cwd" strings in environment */

        _TSCHAR envpfx[] = _T("SystemRoot");
        _TSCHAR *envbuf = NULL;
        int envsize = 0;
        int defined = 0;

        /*
         * Allocate space for command line string
         *  tmp counts the number of bytes in the command line string
         *      including spaces between arguments
         *  An empty string is special -- 2 bytes
         */

        for (vp = argv, tmp = 2; *vp; tmp += (unsigned int)_tcslen(*vp++) + 1) ;

        arg_len = tmp;

        /*
         * Allocate space for the command line plus 2 null bytes
         */

        if ( (*argblk = _calloc_crt(tmp, sizeof(_TSCHAR))) == NULL)
        {
                *envblk = NULL;
                errno = ENOMEM;
                _doserrno = E_nomem;
                return(-1);
        }

        if (_ERRCHECK_EINVAL(_tdupenv_s_crt(&envbuf, NULL, envpfx)) != 0)
        {
                retval = -1;
                goto error;
        }

        envsize = (int)_tcslen(envpfx) + 2;
        if (envbuf != NULL)
        {
            envsize += (int)_tcslen(envbuf);
        }

        /*
         * Allocate space for environment strings
         *  tmp counts the number of bytes in the environment strings
         *      including nulls between strings
         *  Also add "_C_FILE_INFO=" string
         */
        if (envp)
                for (vp = envp, tmp = 2; *vp; tmp += (unsigned int)_tcslen(*vp++) + 1) ;

        /*
         * The _osfile and _osfhnd arrays are passed as binary data in
         * dospawn.c
         */
        cfi_len = 0;    /* no _C_FILE_INFO */

        if (!envp)
                *envblk = NULL;
        else {
                /*
                 * Now that we've decided to pass our own environment block,
                 * compute the size of the "current directory" strings to
                 * propagate to the new environment.
                 */

#ifdef WPRFLAG
                /*
                 * Make sure wide environment exists.
                 */
                if (!_wenvptr)
                {
                        if ((_wenvptr = (wchar_t *)__crtGetEnvironmentStringsW()) == NULL)
                        {
                                retval = -1;
                                goto error;
                        }
                }
#else  /* WPRFLAG */
                if (!_aenvptr)
                {
                        if ((_aenvptr = (char *)__crtGetEnvironmentStringsA()) == NULL)
                        {
                                retval = -1;
                                goto error;
                        }
                }
#endif  /* WPRFLAG */

                /*
                 * search for the first one
                 */
                for (cwd_start = 0;
                     _tenvptr[cwd_start] != _T('\0') &&
                       _tenvptr[cwd_start] != _T('=');
                     cwd_start += (int)_tcslen(&_tenvptr[cwd_start]) + 1)
                {
                }

                /* find the total size of all contiguous ones */
                cwd_end = cwd_start;
                while (_tenvptr[cwd_end+0] == _T('=') &&
                       _tenvptr[cwd_end+1] != _T('\0') &&
                       _tenvptr[cwd_end+2] == _T(':') &&
                       _tenvptr[cwd_end+3] == _T('='))
                {
                        cwd_end += 4 + (int)_tcslen(&_tenvptr[cwd_end+4]) + 1;
                }
                tmp += cwd_end - cwd_start;

                /*
                 * Allocate space for the environment strings plus extra null byte
                 */
                env_len = tmp;

                /*
                 * Check if SystemRoot is already defined in environment provided
                 */
                for (vp = envp; *vp; vp++) {
                        if (_tcsncicmp(*vp, envpfx, _tcslen(envpfx)) == 0) {
                                defined = 1;
                                break;
                        }
                }

                if (!defined)
                        tmp += envsize;

                if( !(*envblk = _calloc_crt(tmp, sizeof(_TSCHAR))) )
                {
                        _free_crt(*argblk);
                        *argblk = NULL;
                        errno = ENOMEM;
                        _doserrno = E_nomem;
                        retval = -1;
                        goto done;
                }
        }

        /*
         * Build the command line by concatenating the argument strings
         * with spaces between, and two null bytes at the end.
         * NOTE: The argv[0] argument is followed by a null.
         */

        cptr = *argblk;
        vp = argv;

        if (!*vp)       /* Empty argument list ? */
                ++cptr; /* just two null bytes */
        else {          /* argv[0] must be followed by a null */
                _ERRCHECK(_tcscpy_s(cptr, arg_len - (cptr - *argblk), *vp));
                cptr += (int)_tcslen(*vp++) + 1;
        }

        while( *vp ) {
                _ERRCHECK(_tcscpy_s(cptr, arg_len - (cptr - *argblk), *vp));
                cptr += (int)_tcslen(*vp++);
                *cptr++ = ' ';
        }

        *cptr = cptr[ -1 ] = _T('\0'); /* remove extra blank, add double null */

        /*
         * Build the environment block by concatenating the environment
         * strings with nulls between and two null bytes at the end
         */

        cptr = *envblk;

        if (envp != NULL) {
                /*
                 * Copy the "cwd" strings to the new environment.
                 */
                memcpy(cptr, &_tenvptr[cwd_start], (cwd_end - cwd_start) * sizeof(_TSCHAR));
                cptr += cwd_end - cwd_start;

                /*
                 * Copy the environment strings from "envp".
                 */
                vp = envp;
                while( *vp ) {
                        _ERRCHECK(_tcscpy_s(cptr, env_len - (cptr - *envblk), *vp));
                        cptr += 1 + (int)_tcslen(*vp++);
                }

                if (!defined) {
                        /*
                         * Copy SystemRoot to the new environment.
                         */
                        _ERRCHECK(_tcscpy_s(cptr, envsize, envpfx));
                        _ERRCHECK(_tcscat_s(cptr, envsize, _T("=")));
                        if (envbuf != NULL)
                        {
                            _ERRCHECK(_tcscat_s(cptr, envsize, envbuf));
                        }
                        cptr += envsize;
                }
        }

        if (cptr != NULL) {
                if (cptr == *envblk) {
                        /*
                         * Empty environment block ... this requires two
                         * nulls.
                         */
                        *cptr++ = _T('\0');
                }
                /*
                 * Extra null terminates the segment
                 */
                *cptr = _T('\0');
        }

        goto done;

error:
        _free_crt(*argblk);
        *argblk = NULL;
        *envblk = NULL;

done:

#ifdef WPRFLAG
        if (_wenvptr)
                _free_crt(_wenvptr);
        _wenvptr = NULL;
#else  /* WPRFLAG */
        if (_aenvptr)
                _free_crt(_aenvptr);
        _aenvptr = NULL;
#endif  /* WPRFLAG */
        if (envbuf)
                _free_crt(envbuf);

        return retval;
}


#ifndef _M_IX86

/***
*int _capture_argv(arglist, static_argv, max_static_entries) - set up argv array
*       for exec?? functions
*
*Purpose:
*       Set up the argv array for the exec?? functions by captures the
*       arguments from the passed va_list into the static_argv array.  If the
*       size of the static_argv array as specified by the max_static_entries
*       parameter is not large enough, then allocates a dynamic array to hold
*       the arguments. Return the address of the final argv array.  If NULL
*       then not enough memory to hold argument array.  If different from
*       static_argv parameter then call must free the return argv array when
*       done with it.
*
*       The scan of the arglist is terminated when a NULL argument is
*       reached. The terminating NULL parameter is stored in the resulting
*       argv array.
*
*Entry:
*       va_list *arglist          - pointer to variable length argument list.
*       _TSCHAR *firstarg            - first argument to store in array
*       _TSCHAR **static_argv        - pointer to static argv to use.
*       size_t max_static_entries - maximum number of entries that can be
*                                   placed in static_argv array.
*
*Exit:
*       returns NULL if no memory.
*       Otherwise returns pointer to argv array.
*       (sometimes calls malloc)
*
*Exceptions:
*
*******************************************************************************/

#ifdef WPRFLAG
_TSCHAR ** __cdecl _wcapture_argv(
#else  /* WPRFLAG */
_TSCHAR ** __cdecl _capture_argv(
#endif  /* WPRFLAG */
        va_list *arglist,
        const _TSCHAR *firstarg,
        _TSCHAR **static_argv,
        size_t max_static_entries
        )
{
        _TSCHAR ** argv;
        _TSCHAR * nextarg;
        size_t i;
        size_t max_entries;

        nextarg = (_TSCHAR *)firstarg;
        argv = static_argv;
        max_entries = max_static_entries;
        i = 0;
        for (;;) {
            if (i >= max_entries) {
                /* The math here looks slightly odd. We really want (max_entries*2)*sizeof(_TSCHAR), but that can overflow
                   We take advantage of calloc's ability to catch overflow and the fact that sizeof(_TSCHAR)*2 cannot overflow
                */
                if (argv == static_argv)
                {
                    argv = _calloc_crt(max_entries, sizeof(_TSCHAR *)*2);
                }
                else
                {
                    argv = _recalloc_crt(argv, max_entries, sizeof(_TSCHAR *)*2);
                }

                if (argv == NULL) break;
                max_entries += max_entries;
            }

            argv[ i++ ] = nextarg;
            if (nextarg == NULL) break;
            nextarg = va_arg(*arglist, _TSCHAR *);
        }

        return argv;
}

#endif  /* _M_IX86 */
