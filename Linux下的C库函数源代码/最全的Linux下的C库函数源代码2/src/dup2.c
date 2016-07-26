/***
*dup2.c - Duplicate file handles
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _dup2() - duplicate file handles
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <oscalls.h>
#include <msdos.h>
#include <mtdll.h>
#include <errno.h>
#include <stdlib.h>
#include <internal.h>
#include <malloc.h>
#include <dbgint.h>

static int __cdecl extend_ioinfo_arrays(int);

static int __cdecl _dup2_nolock(int, int);

/***
*int _dup2(fh1, fh2) - force handle 2 to refer to handle 1
*
*Purpose:
*       Forces file handle 2 to refer to the same file as file
*       handle 1.  If file handle 2 referred to an open file, that file
*       is closed.
*
*       Multi-thread: We must hold 2 lowio locks at the same time
*       to ensure multi-thread integrity.  In order to prevent deadlock,
*       we always get the lower file handle lock first.  Order of unlocking
*       does not matter.  If you modify this routine, make sure you don't
*       cause any deadlocks! Scary stuff, kids!!
*
*Entry:
*       int fh1 - file handle to duplicate
*       int fh2 - file handle to assign to file handle 1
*
*Exit:
*       returns 0 if successful, -1 (and sets errno) if fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _dup2 (
        int fh1,
        int fh2
        )
{
        int retcode;

        /* validate file handles */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh1, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh1 >= 0 && (unsigned)fh1 < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh1) & FOPEN), EBADF, -1);
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh2, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN(((unsigned)fh2 < _NHANDLE_), EBADF, -1);

        /*
         * Make sure there is an ioinfo struct corresponding to fh2.
         */
        if ( (fh2 >= _nhandle) && (extend_ioinfo_arrays(fh2) != 0) ) {
                errno = ENOMEM;
                return -1;
        }


        /* get the two file handle locks; in order to prevent deadlock,
           get the lowest handle lock first. */
        if ( fh1 < fh2 ) {
                _lock_fh(fh1);
                _lock_fh(fh2);
        }
        else if ( fh1 > fh2 ) {
                _lock_fh(fh2);
                _lock_fh(fh1);
        }

        __try {
                retcode = _dup2_nolock(fh1, fh2);
        }
        __finally {
                _unlock_fh(fh1);
                _unlock_fh(fh2);
        }

        return retcode;

}

static int __cdecl _dup2_nolock (
        int fh1,
        int fh2
        )
{

        ULONG dosretval;                /* o.s. return code */
        intptr_t new_osfhandle;

        /*
         * Re-test and take care of case of unopened source handle. This is
         * necessary only in the multi-thread case where the file have been
         * closed by another thread before the lock was asserted, but after
         * the initial test above.
         */
        if ( !(_osfile(fh1) & FOPEN) ) {
                /*
                 * Source handle isn't open, bail out with an error.
                 * Note that the DuplicateHandle API will not detect this
                 * error since it implies that _osfhnd(fh1) ==
                 * INVALID_HANDLE_VALUE, and this is a legal HANDLE value
                 * (it's the HANDLE for the current process).
                 */
                errno = EBADF;
                _doserrno = 0;  /* not an OS error */
                _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                return -1;
        }


        /*
         * Take of the case of equal handles.
         */
        if ( fh1 == fh2 )
                /*
                 * Since fh1 is known to be open, return 0 indicating success.
                 * This is in conformance with the POSIX specification for
                 * dup2.
                 */
                return 0;

        /*
         * if fh2 is open, close it.
         */
        if ( _osfile(fh2) & FOPEN )
                /*
                 * close the handle. ignore the possibility of an error - an
                 * error simply means that an OS handle value may remain bound
                 * for the duration of the process.  Use _close_nolock as we
                 * already own lock
                 */
                (void) _close_nolock(fh2);


        /* Duplicate source file onto target file */

        if ( !(DuplicateHandle(GetCurrentProcess(),
                               (HANDLE)_get_osfhandle(fh1),
                               GetCurrentProcess(),
                               (PHANDLE)&new_osfhandle,
                               0L,
                               TRUE,
                               DUPLICATE_SAME_ACCESS)) )
        {

                dosretval = GetLastError();
        }
        else {
                _set_osfhnd(fh2, new_osfhandle);
                dosretval = 0;
        }

        if (dosretval) {
                _dosmaperr(dosretval);
                return -1;
        }

        /* copy the _osfile information, with the FNOINHERIT bit cleared */
        _osfile(fh2) = _osfile(fh1) & ~FNOINHERIT;
        _textmode(fh2) = _textmode(fh1);
        _tm_unicode(fh2) = _tm_unicode(fh1);

        return 0;
}


/***
*static int extend_ioinfo_arrays( int fh ) - extend ioinfo arrays to fh
*
*Purpose:
*       Allocate and initialize arrays of ioinfo structs,filling in
*       __pioinfo[],until there is an ioinfo struct corresponding to fh.
*
*       Note: It is assumed the fh < _NHANDLE_!
*
*Entry:
*       int fh  - C file handle corresponding to ioinfo
*
*Exit:
*       returns 0 if successful, -1
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl extend_ioinfo_arrays(
        int fh
        )
{
        ioinfo *pio;
        int i;

        /*
         * Walk __pioinfo[], allocating an array of ioinfo structs for each
         * empty entry, until there is an ioinfo struct corresponding to fh.
         */
        for ( i = 0 ; fh >= _nhandle ; i++ ) {

            if ( __pioinfo[i] == NULL ) {

                if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))
                     != NULL )
                {
                    __pioinfo[i] = pio;
                    _nhandle += IOINFO_ARRAY_ELTS;

                    for ( ; pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; pio++ ) {
                        pio->osfile = 0;
                        pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                        pio->pipech = 10;
                        pio->lockinitflag = 0;
                    }
                }
                else {
                    /*
                     * Couldn't allocate another array, return failure.
                     */
                    return -1;
                }
            }
        }

        return 0;
}
