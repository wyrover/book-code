/***
*locking.c - file locking function
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defined the _locking() function - file locking and unlocking
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <errno.h>
#include <sys\locking.h>
#include <io.h>
#include <stdlib.h>
#include <internal.h>
#include <msdos.h>
#include <mtdll.h>

static int __cdecl _locking_nolock(int, int, long);

/***
*int _locking(fh,lmode,nbytes) - file record locking function
*
*Purpose:
*       Locks or unlocks nbytes of a specified file
*
*       Multi-thread - Must lock/unlock the file handle to prevent
*       other threads from working on the file at the same time as us.
*       [NOTE: We do NOT release the lock during the 1 second delays
*       since some other thread could get in and do something to the
*       file.  The DOSFILELOCK call locks out other processes, not
*       threads, so there is no multi-thread deadlock at the DOS file
*       locking level.]
*
*Entry:
*       int fh -        file handle
*       int lmode -     locking mode:
*                           _LK_LOCK/_LK_RLCK -> lock, retry 10 times
*                           _LK_NBLCK/_LK_N_BRLCK -> lock, don't retry
*                           _LK_UNLCK -> unlock
*       long nbytes -   number of bytes to lock/unlock
*
*Exit:
*       returns 0 if successful
*       returns -1 and sets errno if unsuccessful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _locking (
        int fh,
        int lmode,
        long nbytes
        )
{
        int retval;

        /* validate file handle */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);


        _lock_fh(fh);                   /* acquire file handle lock */

        __try {
                if ( _osfile(fh) & FOPEN )
                        retval = _locking_nolock(fh, lmode, nbytes);
                else {
                        errno = EBADF;
                        _doserrno = 0;  /* not an o.s. error */
                        retval = -1;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                }
        }
        __finally {
                _unlock_fh(fh);
        }

        return retval;
}

static int __cdecl _locking_nolock (
        int fh,
        int lmode,
        long nbytes
        )
{
        ULONG dosretval;                /* o.s. return code */
        LONG lockoffset;
        int retry;                      /* retry count */


        /* obtain current position in file by calling _lseek */
        /* Use _lseek_nolock as we already own lock */
        lockoffset = _lseek_nolock(fh, 0L, 1);
        if (lockoffset == -1)
                return -1;

        /* set retry count based on mode */
        if (lmode == _LK_LOCK || lmode == _LK_RLCK)
                retry = 9;              /* retry 9 times */
        else
                retry = 0;              /* don't retry */

        /* ask o.s. to lock the file until success or retry count finished */
        /* note that the only error possible is a locking violation, since */
        /* an invalid handle would have already failed above */
        for (;;) {

                dosretval = 0;
                if (lmode == _LK_UNLCK) {
                    if ( !(UnlockFile((HANDLE)_get_osfhandle(fh),
                                      lockoffset,
                                      0L,
                                      nbytes,
                                      0L))
                       )
                        dosretval = GetLastError();

                } else {
                    if ( !(LockFile((HANDLE)_get_osfhandle(fh),
                                    lockoffset,
                                    0L,
                                    nbytes,
                                    0L))
                       )
                        dosretval = GetLastError();
                }

                if (retry <= 0 || dosretval == 0)
                        break;  /* exit loop on success or retry exhausted */

                Sleep(1000L);

                --retry;
        }

        if (dosretval != 0) {
                /* o.s. error occurred -- file was already locked; if a
                   blocking call, then return EDEADLOCK, otherwise map
                   error normally */
                if (lmode == _LK_LOCK || lmode == _LK_RLCK) {
                        errno = EDEADLOCK;
                        _doserrno = dosretval;
                }
                else {
                        _dosmaperr(dosretval);
                }
                return -1;
        }
        else
                return 0;
}
