/***
*dup.c - duplicate file handles
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _dup() - duplicate file handles
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <errno.h>
#include <mtdll.h>
#include <io.h>
#include <msdos.h>
#include <internal.h>
#include <stdlib.h>

static int __cdecl _dup_nolock(int);

/***
*int _dup(fh) - duplicate a file handle
*
*Purpose:
*       Assigns another file handle to the file associated with the
*       handle fh.  The next available file handle is assigned.
*
*       Multi-thread: Be sure not to hold two file handle locks
*       at the same time!
*
*Entry:
*       int fh - file handle to duplicate
*
*Exit:
*       returns new file handle if successful
*       returns -1 (and sets errno) if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _dup (
        int fh
        )
{
        int newfh =-1;                      /* variable for new file handle */

        /* validate file handle */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);



        _lock_fh(fh);                   /* lock file handle */
        __TRY
                if ( _osfile(fh) & FOPEN )
                        newfh = _dup_nolock(fh);
                else {
                        errno = EBADF;
                        _doserrno = 0;
                        newfh = -1;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                }
        __FINALLY
                _unlock_fh(fh);
        __END_TRY_FINALLY

        return newfh;
}

static int __cdecl _dup_nolock(
        int fh
        )
{
        int newfh;                      /* variable for new file handle */
        ULONG dosretval;                /* o.s. return value */
        char fileinfo;                  /* _osfile info for file */
        intptr_t new_osfhandle;
        int success = FALSE;

        fileinfo = _osfile(fh);         /* get file info for file */

        if ( !(_osfile(fh) & FOPEN) )
                return -1;


        /* create duplicate handle */

        if ( (newfh = _alloc_osfhnd()) == -1 )
        {
                errno = EMFILE;         /* too many files error */
                _doserrno = 0L;         /* not an OS error */
                return -1;              /* return error to caller */
        }
        __TRY

            /*
             * duplicate the file handle
             */
            if ( !(DuplicateHandle(GetCurrentProcess(),
                                   (HANDLE)_get_osfhandle(fh),
                                   GetCurrentProcess(),
                                   (PHANDLE)&new_osfhandle,
                                   0L,
                                   TRUE,
                                   DUPLICATE_SAME_ACCESS)) )
            {
                    dosretval = GetLastError();
            }
            else {
                    _set_osfhnd(newfh, new_osfhandle);
                    dosretval = 0;
            }

            if (dosretval)
                        {
                    /* o.s. error -- map errpr and release handle */
                    _dosmaperr(dosretval);
            }
            else
            {
                    /*
                     * copy the _osfile value, with the FNOINHERIT bit cleared
                     */
                    _osfile(newfh) = fileinfo & ~FNOINHERIT;
                    _textmode(newfh) = _textmode(fh);
                    _tm_unicode(newfh) = _tm_unicode(fh);
                    success = TRUE;
            }

        __FINALLY
            if (!success)
            {
                _osfile(newfh) &= ~FOPEN;
            }
            _unlock_fh(newfh);
        __END_TRY_FINALLY

            return success ? newfh : -1;
}
