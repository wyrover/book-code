/***
*rmtmp.c - remove temporary files created by tmpfile.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <sect_attribs.h>
#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

_CRTALLOC(".CRT$XPX") static _PVFV pterm = _rmtmp;

/*
 * Definitions for _tmpoff, _tempoff and _old_pfxlen. These will cause this
 * module to be linked in whenever the termination code needs it.
 */
#ifndef CRTDLL
unsigned _tmpoff = 1;
#endif  /* CRTDLL */

unsigned _tempoff = 1;
unsigned _old_pfxlen = 0;


/***
*int _rmtmp() - closes and removes temp files created by tmpfile
*
*Purpose:
*       closes and deletes all open files that were created by tmpfile.
*
*Entry:
*       None.
*
*Exit:
*       returns number of streams closed
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _rmtmp (
        void
        )
{
        REG2 int count = 0;
        REG1 int i;

        _mlock(_IOB_SCAN_LOCK);
        __try {

        for ( i = 0 ; i < _nstream ; i++)

                if ( __piob[i] != NULL && inuse( (FILE *)__piob[i] )) {

                        /*
                         * lock the stream. this is not done until testing
                         * the stream is in use to avoid unnecessarily creating
                         * a lock for every stream. the price is having to
                         * retest the stream after the lock has been asserted.
                         */
                        _lock_str2(i, __piob[i]);
                        __try {
                                /*
                                 * if the stream is STILL in use (it may have
                                 * been closed before the lock was asserted),
                                 * see about flushing it.
                                 */
                                if ( inuse( (FILE *)__piob[i] )) {

                        if ( ((FILE *)__piob[i])->_tmpfname != NULL )
                        {
                                _fclose_nolock( __piob[i] );
                                count++;
                        }

                                }
                        }
                        __finally {
                                _unlock_str2(i, __piob[i]);
                        }
                }

        }
        __finally {
                _munlock(_IOB_SCAN_LOCK);
        }

        return(count);
}
