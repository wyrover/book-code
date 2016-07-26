/***
*stdenvp.c - standard _setenvp routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module is called by the C start-up routine to set up "_environ".
*       Its sets up an array of pointers to strings in the environment.
*       The global symbol "_environ" is set to point to this array.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <stdlib.h>
#include <internal.h>
#include <rterr.h>
#include <oscalls.h>
#include <tchar.h>
#include <dbgint.h>

#ifndef CRTDLL

#ifdef _MBCS
/*
 * Flag to ensure multibyte ctype table is only initialized once
 */
extern int __mbctype_initialized;

#endif  /* _MBCS */

/*
 * Flag checked by getenv() and _putenv() to determine if the environment has
 * been initialized.
 */
extern int __env_initialized;

#endif  /* CRTDLL */

/***
*_setenvp - set up "envp" for C programs
*
*Purpose:
*       Reads the environment and build the envp array for C programs.
*
*Entry:
*       The environment strings occur at _aenvptr.
*       The list of environment strings is terminated by an extra null
*       byte.  Thus two null bytes in a row indicate the end of the
*       last environment string and the end of the environment, resp.
*
*Exit:
*       "environ" points to a null-terminated list of pointers to ASCIZ
*       strings, each of which is of the form "VAR=VALUE".  The strings
*       are copied from the environment area. This array of pointers will
*       be malloc'ed.  The block pointed to by _aenvptr is deallocated.
*
*Uses:
*       Allocates space on the heap for the environment pointers.
*
*Exceptions:
*       If space cannot be allocated, program is terminated.
*
*******************************************************************************/

#ifdef WPRFLAG
#define _tsetenvp    _wsetenvp
#define _tenvptr     _wenvptr
#else  /* WPRFLAG */
#define _tsetenvp    _setenvp
#define _tenvptr     _aenvptr
#endif  /* WPRFLAG */

int __cdecl _tsetenvp (
        void
        )
{
        _TSCHAR *p;
        _TSCHAR **env;              /* _environ ptr traversal pointer */
        int numstrings;             /* number of environment strings */
        int cchars;

#if !defined (CRTDLL) && defined (_MBCS)
        /* If necessary, initialize the multibyte ctype table. */
        if ( __mbctype_initialized == 0 )
            __initmbctable();
#endif  /* !defined (CRTDLL) && defined (_MBCS) */

        numstrings = 0;

        p = _tenvptr;

        /*
         * We called __crtGetEnvironmentStrings[AW] just before this,
         * so if _[aw]envptr is NULL, we failed to get the environment.
         * Return an error.
         */
        if (p == NULL)
            return -1;

        /*
         * NOTE: starting with single null indicates no environ.
         * Count the number of strings. Skip drive letter settings
         * ("=C:=C:\foo" type) by skipping all environment variables
         * that begin with '=' character.
         */

        while (*p != _T('\0')) {
            /* don't count "=..." type */
            if (*p != _T('='))
                ++numstrings;
            p += _tcslen(p) + 1;
        }

        /* need pointer for each string, plus one null ptr at end */
        if ( (_tenviron = env = (_TSCHAR **)
            _calloc_crt((numstrings+1), sizeof(_TSCHAR *))) == NULL )
            return -1;

        /* copy strings to malloc'd memory and save pointers in _environ */
        for ( p = _tenvptr ; *p != L'\0' ; p += cchars )
        {
            cchars = (int)_tcslen(p) + 1;
            /* don't copy "=..." type */
            if (*p != _T('=')) {
                if ( (*env = (_TSCHAR *)_calloc_crt(cchars, sizeof(_TSCHAR)))
                     == NULL )
                {
                    _free_crt(_tenviron);
                    _tenviron = NULL;
                    return -1;
                }
                _ERRCHECK(_tcscpy_s(*env, cchars, p));
                env++;
            }
        }

        _free_crt(_tenvptr);
        _tenvptr = NULL;

        /* and a final NULL pointer */
        *env = NULL;

#ifndef CRTDLL
        /*
         * Set flag for getenv() and _putenv() to know the environment
         * has been set up.
         */
        __env_initialized = 1;
#endif  /* CRTDLL */

        return 0;
}
