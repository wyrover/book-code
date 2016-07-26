/***
*clearerr.c - clear error and eof flags
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines clearerr() - clear error and eof flags from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <mtdll.h>
#include <internal.h>
#include <msdos.h>

/***
*errno_t clearerr(stream) - clear error and eof flags on a stream
*
*Purpose:
*       Resets the error and eof indicators for a stream to 0
*
*Entry:
*       FILE *stream - stream to set indicators on
*
*Exit:
*       0 on success, Otherwise error code
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl clearerr_s (FILE * stream)
{
        _VALIDATE_RETURN_ERRCODE((stream != NULL), EINVAL);

        _lock_str(stream);
        __try {

        /* Clear stdio level flags */
        stream->_flag &= ~(_IOERR|_IOEOF);

        /* Clear lowio level flags */

        _osfile_safe(_fileno(stream)) &= ~(FEOFLAG);

        }
        __finally {
            _unlock_str(stream);
        }
    return 0;
}

/***
*void clearerr(stream) - clear error and eof flags on a stream
*
*Purpose:
*       Resets the error and eof indicators for a stream to 0
*
*Entry:
*       FILE *stream - stream to set indicators on
*
*Exit:
*       No return value.
*       changes the _flag field of the FILE struct.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl clearerr (
        FILE *stream
        )
{
    clearerr_s(stream);
}

