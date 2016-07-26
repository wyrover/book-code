/***
*flength.c - find length of a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _filelength() - find the length of a file
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <internal.h>
#include <msdos.h>
#include <mtdll.h>
#include <stddef.h>
#include <stdlib.h>

/***
*long _filelength(filedes) - find length of a file
*
*Purpose:
*       Returns the length in bytes of the specified file.
*
*Entry:
*       int filedes - handle referring to file to find length of
*
*Exit:
*       returns length of file in bytes
*       returns -1L if fails
*
*Exceptions:
*
*******************************************************************************/

long __cdecl _filelength (
        int filedes
        )
{
        long length;
        long here;

        _CHECK_FH_CLEAR_OSSERR_RETURN( filedes, EBADF, -1L );
        _VALIDATE_CLEAR_OSSERR_RETURN((filedes >= 0 && (unsigned)filedes < (unsigned)_nhandle), EBADF, -1L);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(filedes) & FOPEN), EBADF, -1L);

        _lock_fh(filedes);
        __try {
                if ( _osfile(filedes) & FOPEN ) {

        /* Seek to end to get length of file. */
        if ( (here = _lseek_nolock(filedes, 0L, SEEK_CUR)) == -1L )
                length = -1L;   /* return error */
        else {
                length = _lseek_nolock(filedes, 0L, SEEK_END);
                if ( here != length )
                        _lseek_nolock(filedes, here, SEEK_SET);
        }

                }
                else {
                        errno = EBADF;
                        _doserrno = 0L;
                        length = -1L;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                }
        }
        __finally {
                _unlock_fh(filedes);
        }

        return(length);
}
