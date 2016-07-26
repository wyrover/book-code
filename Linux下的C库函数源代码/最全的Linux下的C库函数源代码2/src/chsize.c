/***
*chsize.c - change size of a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains the _chsize() function - changes the size of a file.
*
*******************************************************************************/

#include <cruntime.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dbgint.h>
#include <fcntl.h>
#include <msdos.h>
#include <io.h>
#include <string.h>
#include <windows.h>
#include <internal.h>
#include <mtdll.h>

/***
*erccode _chsize_s(filedes, size) - change size of a file
*
*Purpose:
*       Change file size. Assume file is open for writing, or we can't do it.
*       The DOS way to do this is to go to the right spot and write 0 bytes. The
*       Xenix way to do this is to make a system call. We write '\0' bytes because
*       DOS won't do this for you if you lseek beyond eof, though Xenix will.
*
*Entry:
*       int filedes - file handle to change size of
*       __int64 size - new size of file
*
*Exit:
*       return 0 if successful
*       returns errno_t on failure
*
*Exceptions:
*
*******************************************************************************/


/* define normal version that locks/unlocks, validates fh */

errno_t __cdecl _chsize_s (
        REG1 int filedes,
        __int64 size
        )
{
        int r;                          /* return value */

        _CHECK_FH_CLEAR_OSSERR_RETURN_ERRCODE( filedes, EBADF );
        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((filedes >= 0 && (unsigned)filedes < (unsigned)_nhandle), EBADF);
        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((_osfile(filedes) & FOPEN), EBADF);
        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((size >= 0), EINVAL);

        _lock_fh(filedes);

        __try {
                if ( _osfile(filedes) & FOPEN )
                        r = _chsize_nolock(filedes,size);
                else {
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                        r = errno = EBADF;
                }
        }
        __finally {
                _unlock_fh(filedes);
        }

        return r;
}

/* now define version that doesn't lock/unlock, validate fh */
errno_t __cdecl _chsize_nolock (
        REG1 int filedes,
        __int64 size
        )
{
        __int64 filend;
        __int64 extend;
        __int64 place;
        int cnt;
        int oldmode;
        __int64 retval = 0; /* assume good return */
        errno_t err = 0;



        /* Get current file position and seek to end */
        if ( ((place = _lseeki64_nolock(filedes, 0i64, SEEK_CUR)) == -1i64) ||
             ((filend = _lseeki64_nolock(filedes, 0i64, SEEK_END)) == -1i64) )
            return errno;

        extend = size - filend;

        /* Grow or shrink the file as necessary */

        if (extend > 0i64) {

            /* extending the file */
            char *bl = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, _INTERNAL_BUFSIZ);

            if (!bl) {
                err = errno = ENOMEM;
                retval= -1i64;
            } else {
                oldmode = _setmode_nolock(filedes, _O_BINARY);

                /* pad out with nulls */
                do  {
                    cnt = (extend >= (__int64)_INTERNAL_BUFSIZ ) ?
                          _INTERNAL_BUFSIZ : (int)extend;
                    if ( (cnt = _write_nolock( filedes,
                                           bl,
                                           (extend >= (__int64)_INTERNAL_BUFSIZ) ?
                                                _INTERNAL_BUFSIZ : (int)extend ))
                         == -1 )
                    {
                        /* Error on write */
                        if (_doserrno == ERROR_ACCESS_DENIED)
                            err = errno = EACCES;

                        retval = cnt;
                        break;  /* leave write loop */
                    }
                }
                while ((extend -= (__int64)cnt) > 0i64);

                _setmode_nolock(filedes, oldmode);

                HeapFree(GetProcessHeap(), 0, bl);
            }

            /* retval set correctly */
        }

        else  if ( extend < 0i64 ) {
            /* shortening the file */

            /*
             * Set file pointer to new eof...and truncate it there.
             */
            retval = _lseeki64_nolock(filedes, size, SEEK_SET);


            if(retval != -1i64)
            {
                if ( (retval = SetEndOfFile((HANDLE)_get_osfhandle(filedes)) ?
                     0 : -1) == -1 )
                {
                    err = errno = EACCES;
                    _doserrno = GetLastError();
                }
            }
        }

        /* else */
        /* no file change needed */
        /* retval = 0; */


/* Common return code */

        if(retval == -1 || (_lseeki64_nolock(filedes, place, SEEK_SET) == -1i64))
        {
            return errno;
        }

        return 0;
}

/***
*int _chsize(filedes, size) - change size of a file
*
*Purpose:
*       Change file size. Assume file is open for writing, or we can't do it.
*       The DOS way to do this is to go to the right spot and write 0 bytes. The
*       Xenix way to do this is to make a system call. We write '\0' bytes because
*       DOS won't do this for you if you lseek beyond eof, though Xenix will.
*
*Entry:
*       int filedes - file handle to change size of
*       long size - new size of file
*
*Exit:
*       return 0 if successful
*       returns -1 and sets errno if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _chsize (
        REG1 int filedes,
        long size
        )
{
    errno_t e;
    e = _chsize_s(filedes, (__int64)size) ;

    return e == 0 ? 0 : -1 ;
}
