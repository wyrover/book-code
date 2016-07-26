/***
*stream.c - find a stream not in use
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getstream() - find a stream not in use
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>
#include <dbgint.h>

/***
*FILE *_getstream() - find a stream not in use
*
*Purpose:
*       Find a stream not in use and make it available to caller. Intended
*       for use inside library only
*
*Entry:
*       None. Scans __piob[]
*
*Exit:
*       Returns a pointer to a free stream, or NULL if all are in use.  A
*       stream becomes allocated if the caller decided to use it by setting
*       any r, w, r/w mode.
*
*       [Multi-thread note: If a free stream is found, it is returned in a
*       LOCKED state.  It is the caller's responsibility to unlock the stream.]
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _getstream (
        void
        )
{
        REG2 FILE *retval = NULL;
        REG1 int i;

        /* Get the iob[] scan lock */
        _mlock(_IOB_SCAN_LOCK);
        __try {

        /*
         * Loop through the __piob table looking for a free stream, or the
         * first NULL entry.
         */
        for ( i = 0 ; i < _nstream ; i++ ) {

            if ( __piob[i] != NULL ) {
                /*
                 * if the stream is not inuse, return it.
                 */
                if ( !inuse( (FILE *)__piob[i] ) ) {
                    /*
                     * Allocate the FILE lock, in case it hasn't already been
                     * allocated (only necessary for the first _IOB_ENTRIES
                     * locks, not including stdin/stdout/stderr).  Return
                     * failure if lock can't be allocated.
                     */
                    if ( i > 2 && i < _IOB_ENTRIES )
                        if ( !_mtinitlocknum( _STREAM_LOCKS + i ) )
                            break;

                    _lock_str2(i, __piob[i]);

                    if ( inuse( (FILE *)__piob[i] ) ) {
                        _unlock_str2(i, __piob[i]);
                        continue;
                    }
                    retval = (FILE *)__piob[i];
                    break;
                }
            }
            else {
                /*
                 * allocate a new _FILEX, set _piob[i] to it and return a
                 * pointer to it.
                 */
                if ( (__piob[i] = _malloc_crt( sizeof(_FILEX) )) != NULL ) {

                    if ( !__crtInitCritSecAndSpinCount(
                         &(((_FILEX *)__piob[i])->lock), _CRT_SPINCOUNT ))
                    {
                        /*
                         * Failed to initialize the critical section because
                         * of lack of memory, clean up and return failure.
                         */
                        _free_crt( __piob[i] );
                        __piob[i] = NULL;
                        break;
                    }

                    EnterCriticalSection( &(((_FILEX *)__piob[i])->lock) );
                    retval = (FILE *)__piob[i];
                }

                break;
            }
        }

        /*
         * Initialize the return stream.
         */
        if ( retval != NULL ) {
            retval->_flag = retval->_cnt = 0;
            retval->_tmpfname = retval->_ptr = retval->_base = NULL;
            retval->_file = -1;
        }

        }
        __finally {
            _munlock(_IOB_SCAN_LOCK);
        }

        return(retval);
}
