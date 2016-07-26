/***
*rewind.c - rewind a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines rewind() - rewinds a stream to the beginning.
*
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <dbgint.h>
#include <io.h>
#include <mtdll.h>
#include <msdos.h>
#include <internal.h>

/***
*void rewind(stream) - rewind a string
*
*Purpose:
*       Back up a stream to the beginning (if not terminal).  First flush it.
*       If read/write, allow next i/o operation to set mode.
*
*Entry:
*       FILE *stream - file to rewind
*
*Exceptions:
*
*******************************************************************************/

void __cdecl rewind (
        FILE *str
        )
{
        REG1 FILE *stream;
        REG2 int fd;

        _VALIDATE_RETURN_VOID( (str != NULL), EINVAL);

        /* Init stream pointer */
        stream = str;

        fd = _fileno(stream);

        /* Lock the file */
        _lock_str(stream);
        __try {

        /* Flush the stream */
        _flush(stream);

        /* Clear errors */
        stream->_flag &= ~(_IOERR|_IOEOF);
        _osfile_safe(fd) &= ~(FEOFLAG);

        /* Set flags */
        /* [note: _flush set _cnt=0 and _ptr=_base] */
        if (stream->_flag & _IORW)
            stream->_flag &= ~(_IOREAD|_IOWRT);

        /* Position to beginning of file */
        if(_lseek(fd,0L,0)==-1)
                {
                        stream->_flag |= _IOERR;
                }

        }
        __finally {
            /* unlock stream */
            _unlock_str(stream);
        }

}
