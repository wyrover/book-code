/***
*osfinfo.c - Win32 _osfhnd[] support routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the internally used routine _alloc_osfhnd()
*       and the user visible routine _get_osfhandle().
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <errno.h>
#include <internal.h>
#include <fcntl.h>
#include <malloc.h>
#include <msdos.h>
#include <mtdll.h>
#include <stdlib.h>
#include <dbgint.h>
#include <io.h>


/***
*int _alloc_osfhnd() - get free _ioinfo struct
*
*Purpose:
*       Finds the first free entry in the arrays of ioinfo structs and
*       returns the index of that entry (which is the CRT file handle to the
*       caller) to the caller.  The FOPEN flag is turned on in the new entry,
*       to prevent multithreaded race conditions/deadlocks.
*
*Entry:
*       none
*
*Exit:
*       returns index of the entry, if successful
*       return -1, if no free entry is available or out of memory
*
*       MULTITHREAD NOTE: IF SUCCESSFUL, THE HANDLE IS LOCKED WHEN IT IS
*       RETURNED TO THE CALLER!  ALSO, FOPEN HAS BEEN TURNED ON, SO IF THE
*       CALLER ABANDONS THE HANDLE, FOPEN MUST BE CLEARED!
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _alloc_osfhnd(
        void
        )
{
        int fh = -1;    /* file handle */
        int i;
        ioinfo *pio;
        int failed=FALSE;

        if (!_mtinitlocknum(_OSFHND_LOCK))
            return -1;

        _mlock(_OSFHND_LOCK);   /* lock the __pioinfo[] array */
        __TRY

            /*
             * Search the arrays of ioinfo structs, in order, looking for the
             * first free entry. The compound index of this free entry is the
             * return value. Here, the compound index of the ioinfo struct
             * *(__pioinfo[i] + j) is k = i * IOINFO_ARRAY_ELTS + j, and k = 0,
             * 1, 2,... is the order of the search.
             */
            for ( i = 0 ; i < IOINFO_ARRAYS ; i++ ) {
                /*
                 * If __pioinfo[i] is non-empty array, search it looking for
                 * the first free entry. Otherwise, allocate a new array and use
                 * its first entry.
                 */
                if ( __pioinfo[i] != NULL ) {
                    /*
                     * Search for an available entry.
                     */
                    for ( pio = __pioinfo[i] ;
                          pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ;
                          pio++ )
                    {
                        if ( (pio->osfile & FOPEN) == 0 ) {
                            /*
                             * Make sure the lock is initialized.
                             */
                            if ( pio->lockinitflag == 0 ) {
                                _mlock( _LOCKTAB_LOCK );
                                __TRY
                                    if ( pio->lockinitflag == 0 ) {
                                        if ( !__crtInitCritSecAndSpinCount( &(pio->lock), _CRT_SPINCOUNT ))
                                        {
                                            /*
                                             * Lock initialization failed.  Release
                                             * held locks and return failure.
                                            */
                                            failed=TRUE;
                                        }
                                        else
                                        {
                                            pio->lockinitflag++;
                                        }
                                    }
                                __FINALLY
                                    _munlock( _LOCKTAB_LOCK );
                                __END_TRY_FINALLY
                            }

                            if(!failed)
                            {
                                EnterCriticalSection( &(pio->lock) );

                                /*
                                * Check for the case where another thread has
                                * managed to grab the handle out from under us.
                                */
                                if ( (pio->osfile & FOPEN) != 0 ) {
                                        LeaveCriticalSection( &(pio->lock) );
                                        continue;
                                }
                            }
                            if(!failed)
                            {
                                pio->osfile = FOPEN;
                                pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                                fh = i * IOINFO_ARRAY_ELTS + (int)(pio - __pioinfo[i]);
                                break;
                            }
                        }
                    }

                    /*
                     * Check if a free entry has been found.
                     */
                    if ( fh != -1 )
                        break;
                }
                else {
                    /*
                     * Allocate and initialize another array of ioinfo structs.
                     */
                    if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))
                        != NULL )
                    {

                        /*
                         * Update __pioinfo[] and _nhandle
                         */
                        __pioinfo[i] = pio;
                        _nhandle += IOINFO_ARRAY_ELTS;

                        for ( ; pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; pio++ ) {
                            pio->osfile = 0;
                            pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                            pio->pipech = 10;
                            pio->lockinitflag = 0;
                        }

                        /*
                         * The first element of the newly allocated array of ioinfo
                         * structs, *(__pioinfo[i]), is our first free entry.
                         */
                        fh = i * IOINFO_ARRAY_ELTS;
                        _osfile(fh) = FOPEN;
                        if ( !__lock_fhandle( fh ) ) {
                            /*
                             * The lock initialization failed, return the failure
                             */
                            fh = -1;
                        }
                    }

                    break;
                }
            }
        __FINALLY
            _munlock(_OSFHND_LOCK); /* unlock the __pioinfo[] table */
        __END_TRY_FINALLY

        /*
         * return the index of the previously free table entry, if one was
         * found. return -1 otherwise.
         */
        return( fh );
}


/***
*int _set_osfhnd(int fh, long value) - set Win32 HANDLE value
*
*Purpose:
*       If fh is in range and if _osfhnd(fh) is marked with
*       INVALID_HANDLE_VALUE then set _osfhnd(fh) to the passed value.
*
*Entry:
*       int fh      - CRT file handle
*       long value  - new Win32 HANDLE value for this handle
*
*Exit:
*       Returns zero if successful.
*       Returns -1 and sets errno to EBADF otherwise.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _set_osfhnd (
        int fh,
        intptr_t value
        )
{
        if ( fh >= 0 && ((unsigned)fh < (unsigned)_nhandle) &&
             (_osfhnd(fh) == (intptr_t)INVALID_HANDLE_VALUE)
           ) {
            if ( __app_type == _CONSOLE_APP ) {
                switch (fh) {
                case 0:
                    SetStdHandle( STD_INPUT_HANDLE, (HANDLE)value );
                    break;
                case 1:
                    SetStdHandle( STD_OUTPUT_HANDLE, (HANDLE)value );
                    break;
                case 2:
                    SetStdHandle( STD_ERROR_HANDLE, (HANDLE)value );
                    break;
                }
            }

            _osfhnd(fh) = value;
            return(0);
        } else {
            errno = EBADF;      /* bad handle */
            _doserrno = 0L;     /* not an OS error */
            return -1;
        }
}


/***
*int _free_osfhnd(int fh) - mark osfhnd field of ioinfo struct as free
*
*Purpose:
*       If fh is in range, the corrsponding ioinfo struct is marked as
*       being open, and the osfhnd field is NOT set to INVALID_HANDLE_VALUE,
*       then mark it with INVALID_HANDLE_VALUE.
*
*Entry:
*       int fh -    CRT file handle
*
*Exit:
*       Returns zero if successful.
*       Returns -1 and sets errno to EBADF otherwise.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _free_osfhnd (
        int fh      /* user's file handle */
        )
{
        if ( (fh >= 0 && (unsigned)fh < (unsigned)_nhandle) &&
             (_osfile(fh) & FOPEN) &&
             (_osfhnd(fh) != (intptr_t)INVALID_HANDLE_VALUE) )
        {
            if ( __app_type == _CONSOLE_APP ) {
                switch (fh) {
                case 0:
                    SetStdHandle( STD_INPUT_HANDLE, NULL );
                    break;
                case 1:
                    SetStdHandle( STD_OUTPUT_HANDLE, NULL );
                    break;
                case 2:
                    SetStdHandle( STD_ERROR_HANDLE, NULL );
                    break;
                }
            }

            _osfhnd(fh) = (intptr_t)INVALID_HANDLE_VALUE;
            return(0);
        } else {
            errno = EBADF;      /* bad handle */
            _doserrno = 0L;     /* not an OS error */
            return -1;
        }
}


/***
*long _get_osfhandle(int fh) - get Win32 HANDLE value
*
*Purpose:
*       If fh is in range and marked open, return _osfhnd(fh).
*
*Entry:
*       int fh  - CRT file handle
*
*Exit:
*       Returns the Win32 HANDLE successful.
*       Returns -1 and sets errno to EBADF otherwise.
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _get_osfhandle (
        int fh      /* user's file handle */
        )
{
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);

        return( _osfhnd(fh) );
}

/***
*int _open_osfhandle(long osfhandle, int flags) - open C Runtime file handle
*
*Purpose:
*       This function allocates a free C Runtime file handle and associates
*       it with the Win32 HANDLE specified by the first parameter.
*
*Entry:
*       long osfhandle - Win32 HANDLE to associate with C Runtime file handle.
*       int flags      - flags to associate with C Runtime file handle.
*
*Exit:
*       returns index of entry in fh, if successful
*       return -1, if no free entry is found
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _open_osfhandle(
        intptr_t osfhandle,
        int flags
        )
{
        int fh;
        char fileflags;         /* _osfile flags */
        DWORD isdev;            /* device indicator in low byte */
        int success = FALSE;

        /* copy relevant flags from second parameter */

        fileflags = 0;

        if ( flags & _O_APPEND )
            fileflags |= FAPPEND;

        if ( flags & _O_TEXT )
            fileflags |= FTEXT;

        if ( flags & _O_NOINHERIT )
            fileflags |= FNOINHERIT;

        /* find out what type of file (file/device/pipe) */

        isdev = GetFileType((HANDLE)osfhandle);
        if (isdev == FILE_TYPE_UNKNOWN) {
            /* OS error */
            _dosmaperr( GetLastError() );   /* map error */
            return -1;
        }

        /* is isdev value to set flags */
        if (isdev == FILE_TYPE_CHAR)
            fileflags |= FDEV;
        else if (isdev == FILE_TYPE_PIPE)
            fileflags |= FPIPE;


        /* attempt to allocate a C Runtime file handle */

        if ( (fh = _alloc_osfhnd()) == -1 ) {
            errno = EMFILE;         /* too many open files */
            _doserrno = 0L;         /* not an OS error */
            return -1;              /* return error to caller */
        }
        __TRY

            /*
             * the file is open. now, set the info in _osfhnd array
             */

            _set_osfhnd(fh, osfhandle);

            fileflags |= FOPEN;     /* mark as open */

            _osfile(fh) = fileflags;    /* set osfile entry */
            _textmode(fh) = 0;
            _tm_unicode(fh) = 0;

            success = TRUE;

        __FINALLY
            if (!success)
            {
                _osfile(fh) &= ~FOPEN;
            }
            _unlock_fh(fh);         /* unlock handle */
        __END_TRY_FINALLY

            return success ? fh : -1;   /* return handle */
}



/***
*void __lock_fhandle(int fh) - lock file handle
*
*Purpose:
*       Assert the lock associated with the passed file handle.
*
*Entry:
*       int fh  - CRT file handle
*
*Exit:
*       Returns FALSE if the attempt to initialize the lock fails.  This can
*       only happen the first time the lock is taken, so the return status only
*       needs to be checked on the first such attempt, which is always in
*       _alloc_osfhnd (except for inherited or standard handles, and the lock
*       is allocated manually in _ioinit for those).
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __lock_fhandle (
        int fh
        )
{
        ioinfo *pio = _pioinfo(fh);
        int retval=TRUE;

        /*
         * Make sure the lock has been initialized.
         */
        if ( pio->lockinitflag == 0 ) {

            _mlock( _LOCKTAB_LOCK );
            __TRY
                if ( pio->lockinitflag == 0 ) {
                    if ( !__crtInitCritSecAndSpinCount( &(pio->lock), _CRT_SPINCOUNT )) {
                        /*
                         * Failed to initialize the lock, so return failure code.
                         */
                        retval=FALSE;
                    }
                    pio->lockinitflag++;
                }
            __FINALLY
                _munlock( _LOCKTAB_LOCK);
            __END_TRY_FINALLY
        }

        if(retval)
        {
            EnterCriticalSection( &(_pioinfo(fh)->lock) );
        }

        return retval;
}


/***
*void _unlock_fhandle(int fh) - unlock file handle
*
*Purpose:
*       Release the lock associated with passed file handle.
*
*Entry:
*       int fh  - CRT file handle
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _unlock_fhandle (
        int fh
        )
{
        LeaveCriticalSection( &(_pioinfo(fh)->lock) );
}

