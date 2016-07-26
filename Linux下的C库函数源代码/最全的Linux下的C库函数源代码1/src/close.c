/***
*close.c - close file handle for Windows NT
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _close() - close a file handle
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <io.h>
#include <mtdll.h>
#include <errno.h>
#include <stdlib.h>
#include <msdos.h>
#include <internal.h>

/***
*int _close(fh) - close a file handle
*
*Purpose:
*       Closes the file associated with the file handle fh.
*
*Entry:
*       int fh - file handle to close
*
*Exit:
*       returns 0 if successful, -1 (and sets errno) if fails
*
*Exceptions:
*
*******************************************************************************/


/* define normal version that locks/unlocks, validates fh */

int __cdecl _close (
        int fh
        )
{
        int r;                          /* return value */

        /* validate file handle */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);

        _lock_fh(fh);                   /* lock file */

        __try {
                if ( _osfile(fh) & FOPEN )
                        r = _close_nolock(fh);
                else {
                        errno = EBADF;
                        r = -1;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                }
        }
        __finally {
                _unlock_fh(fh);         /* unlock the file */
        }

        return r;
}

/* now define version that doesn't lock/unlock, validate fh */
int __cdecl _close_nolock (
        int fh
        )
{
        DWORD dosretval;

        /*
         * Close the underlying OS file handle. Special cases:
         *      1. If _get_osfhandle(fh) is INVALID_HANDLE_VALUE, don't try
         *         to actually close it. Just reset the lowio info so the
         *         handle can be reused. The standard handles are setup like
         *         this in Windows app, or a background app.
         *      2. If fh is STDOUT or STDERR, and if STDOUT and STDERR are
         *         mapped to the same OS file handle, skip the CloseHandle
         *         is skipped (without error). STDOUT and STDERR are the only
         *         handles for which this support is provided. Other handles
         *         are mapped to the same OS file handle only at the
         *         programmer's risk.
         */

        if ( (_get_osfhandle(fh) == (intptr_t)INVALID_HANDLE_VALUE) ||
             (((fh == 1 && (_osfile(2) & FOPEN)) ||
               (fh == 2 && (_osfile(1) & FOPEN))) &&
              (_get_osfhandle(1) == _get_osfhandle(2)) ) ||
             CloseHandle( (HANDLE)_get_osfhandle(fh) ) )
        {

                dosretval = 0L;
        }
        else
                dosretval = GetLastError();

        _free_osfhnd(fh);

        _osfile(fh) = 0;                /* clear file flags */

        if (dosretval) {
                /* OS error */
                _dosmaperr(dosretval);
                return -1;
        }

        return 0;                       /* good return */
}
