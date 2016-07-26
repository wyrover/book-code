/***
*_file.c - Definition of _iob[], initializer and terminator.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _iob[], the array of stdio control structures, the initializer
*       and terminator routines, and the multithread locking for stdio.
*
*******************************************************************************/

#include <sect_attribs.h>
#include <cruntime.h>
#include <windows.h>
#include <stdio.h>
#include <file2.h>
#include <internal.h>
#include <malloc.h>
#include <rterr.h>
#include <dbgint.h>
#include <mtdll.h>

/*
 * Buffer for stdin.
 */

char _bufin[_INTERNAL_BUFSIZ];

/*
 * FILE descriptors; preset for stdin/out/err (note that the __tmpnum field
 * is not initialized)
 */
FILE _iob[_IOB_ENTRIES] = {
        /* _ptr, _cnt, _base,  _flag, _file, _charbuf, _bufsiz */

        /* stdin (_iob[0]) */

        { _bufin, 0, _bufin, _IOREAD | _IOYOURBUF, 0, 0, _INTERNAL_BUFSIZ },

        /* stdout (_iob[1]) */

        { NULL, 0, NULL, _IOWRT, 1, 0, 0 },

        /* stderr (_iob[3]) */

        { NULL, 0, NULL, _IOWRT, 2, 0, 0 },

};


/* These functions are for enabling STATIC_CPPLIB functionality */
_CRTIMP FILE * __cdecl __iob_func(void)
{
    return _iob;
}


/*
 * Pointer to array of FILE * or _FILEX * structures.
 */
void ** __piob;

/*
 * Number of open streams (set to _NSTREAM by default)
 */
#ifdef CRTDLL
int _nstream = _NSTREAM_;
#else  /* CRTDLL */
int _nstream;
#endif  /* CRTDLL */


/*
 * Initializer and terminator for stdio
 */
int  __cdecl __initstdio(void);
void __cdecl __endstdio(void);

#ifdef _MSC_VER

_CRTALLOC(".CRT$XIC") static _PIFV pinit = __initstdio;

_CRTALLOC(".CRT$XPXA") static _PVFV pterm = __endstdio;

#endif  /* _MSC_VER */

#ifndef CRTDLL
/*
 * _cflush is a dummy variable used to pull in _endstdio() when any STDIO
 * routine is included in the user program.
 */
int _cflush = 0;
#endif  /* CRTDLL */


/***
* __initstdio - Initialize the stdio system
*
*Purpose:
*       Create and initialize the __piob array.
*
*Entry: <void>
*
*Exit:  Returns _RT_STDIOINIT if error encountered.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __initstdio(void)
{
        int i;

#ifndef CRTDLL
        /*
         * If the user has not supplied a definition of _nstream, set it
         * to _NSTREAM_. If the user has supplied a value that is too small
         * set _nstream to the minimum acceptable value (_IOB_ENTRIES).
         */
        if ( _nstream ==  0 )
            _nstream = _NSTREAM_;
        else if ( _nstream < _IOB_ENTRIES )
            _nstream = _IOB_ENTRIES;
#endif  /* CRTDLL */

        /*
         * Allocate the __piob array. Try for _nstream entries first. If this
         * fails then reset _nstream to _IOB_ENTRIES and try again. If it
         * still fails, bail out with an RTE.
         */
        if ( (__piob = (void **)_calloc_crt( _nstream, sizeof(void *) )) ==
             NULL ) {
            _nstream = _IOB_ENTRIES;
            if ( (__piob = (void **)_calloc_crt( _nstream, sizeof(void *) ))
                 == NULL )
                return _RT_STDIOINIT;
        }

        /*
         * Initialize the first _IOB_ENTRIES to point to the corresponding
         * entries in _iob[].
         */
        for ( i = 0 ; i < _IOB_ENTRIES ; i++ )
            __piob[i] = (void *)&_iob[i];

        for ( i = 0 ; i < 3 ; i++ ) {
            if ( (_osfhnd(i) == (intptr_t)INVALID_HANDLE_VALUE) ||
                 (_osfhnd(i) == _NO_CONSOLE_FILENO) ||
                 (_osfhnd(i) == 0) )
            {
            /*
             * For stdin, stdout & stderr, we use _NO_CONSOLE_FILENO (a value
             * different from _INVALID_HANDLE_VALUE to distinguish between
             * a failure in opening a file & a program run without a console.
             */
                _iob[i]._file = _NO_CONSOLE_FILENO;
            }
        }

        return 0;
}


/***
* __endstdio - Terminate the stdio system
*
*Purpose:
*       Terminate the stdio system
*
*       (1) Flush all streams.  (Do this even if we're going to
*       call fcloseall since that routine won't do anything to the
*       std streams.)
*
*       (2) If returning to caller, close all streams.  This is
*       not necessary if the exe is terminating because the OS will
*       close the files for us (much more efficiently, too).
*
*Entry: <void>
*
*Exit:  <void>
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl __endstdio(void)
{
        /* flush all streams */
        _flushall();

        /* if in callable exit, close all streams */
        if (_exitflag)
                _fcloseall();
        _free_crt(__piob);
}



/***
* _lock_file - Lock a FILE
*
*Purpose:
*       Assert the lock for a stdio-level file
*
*Entry:
*       pf = __piob[] entry (pointer to a FILE or _FILEX)
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _lock_file (
        FILE *pf
        )
{
        /*
         * The way the FILE (pointed to by pf) is locked depends on whether
         * it is part of _iob[] or not
         */
        if ( (pf >= _iob) && (pf <= (&_iob[_IOB_ENTRIES-1])) )
            /*
             * FILE lies in _iob[] so the lock lies in _locktable[].
             */
            _lock( _STREAM_LOCKS + (int)(pf - _iob) );
        else
            /*
             * Not part of _iob[]. Therefore, *pf is a _FILEX and the
             * lock field of the struct is an initialized critical
             * section.
             */
            EnterCriticalSection( &(((_FILEX *)pf)->lock) );
}


/***
* _lock_file2(i, s) - Lock the FILE
*
*Purpose:
*       Assert the lock for a stdio-level file given by s == __piob[i].
*
*Entry:
*       s == __piob[i]
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _lock_file2 (
        int i,
        void *s
        )
{
        /*
         * The way the FILE is locked depends on whether it is part of _iob[]
         * _iob[] or not
         */
        if ( i < _IOB_ENTRIES )
            /*
             * FILE lies in _iob[] so the lock lies in _locktable[].
             */
            _lock( _STREAM_LOCKS + i );
        else
            /*
             * Not part of _iob[]. Therefore, *s is a _FILEX and the
             * lock field of the struct is an initialized critical
             * section.
             */
            EnterCriticalSection( &(((_FILEX *)s)->lock) );
}


/***
* _unlock_file - Unlock a FILE
*
*Purpose:
*       Release the lock for a stdio-level file
*
*Entry:
*       pf = __piob[] entry (pointer to a FILE or _FILEX)
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _unlock_file (
        FILE *pf
        )
{
        /*
         * The way the FILE (pointed to by pf) is unlocked depends on whether
         * it is part of _iob[] or not
         */
        if ( (pf >= _iob) && (pf <= (&_iob[_IOB_ENTRIES-1])) )
            /*
             * FILE lies in _iob[] so the lock lies in _locktable[].
             */
            _unlock( _STREAM_LOCKS + (int)(pf - _iob) );
        else
            /*
             * Not part of _iob[]. Therefore, *pf is a _FILEX and the
             * lock field of the struct is an initialized critical
             * section.
             */
            LeaveCriticalSection( &(((_FILEX *)pf)->lock) );
}


/***
* _unlock_file2(i, s) - Lock the FILE
*
*Purpose:
*       Release the lock for a stdio-level file given by s == __piob[i].
*
*Entry:
*       s == __piob[i]
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _unlock_file2 (
        int i,
        void *s
        )
{
        /*
         * The way the FILE is locked depends on whether it is part of _iob[]
         * _iob[] or not
         */
        if ( i < _IOB_ENTRIES )
            /*
             * FILE lies in _iob[] so the lock lies in _locktable[].
             */
            _unlock( _STREAM_LOCKS + i );
        else
            /*
             * Not part of _iob[]. Therefore, *s is a _FILEX and the
             * lock field of the struct is an initialized critical
             * section.
             */
            LeaveCriticalSection( &(((_FILEX *)s)->lock) );
}

