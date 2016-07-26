/***
*closeall.c - close all open files
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fcloseall() - closes all open files except stdin, stdout
*       stdprn, stderr, and stdaux.
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <stdio.h>
#include <file2.h>
#include <internal.h>
#include <malloc.h>
#include <mtdll.h>
#include <dbgint.h>


/***
*int _fcloseall() - close all open streams
*
*Purpose:
*       Closes all streams currently open except for stdin/out/err/aux/prn.
*       tmpfile() files are among those closed.
*
*Entry:
*       None.
*
*Exit:
*       returns number of streams closed if OK
*       returns EOF if fails.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fcloseall (
        void
        )
{
        REG2 int count = 0;
        REG1 i;

        _mlock(_IOB_SCAN_LOCK);
        __try {

        for ( i = 3 ; i < _nstream ; i++ ) {

            if ( __piob[i] != NULL ) {
                /*
                 * if the stream is in use, close it
                 */
                if ( inuse( (FILE *)__piob[i] ) && (fclose( __piob[i] ) !=
                     EOF) )
                        count++;

                /*
                 * if stream is part of a _FILEX we allocated, free it.
                 */
                if ( i >= _IOB_ENTRIES ) {

                    DeleteCriticalSection( &(((_FILEX *)__piob[i])->lock) );
                    _free_crt( __piob[i] );
                    __piob[i] = NULL;
                }
            }
        }

        }
        __finally {
            _munlock(_IOB_SCAN_LOCK);
        }

        return(count);
}
