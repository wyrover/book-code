/***
*setbuf.c - give new file buffer
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines setbuf() - given a buffer to a stream or make it unbuffered
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>

/***
*void setbuf(stream, buffer) - give a buffer to a stream
*
*Purpose:
*       Allow user to assign his/her own buffer to a stream.
*               if buffer is not NULL, it must be BUFSIZ in length.
*               if buffer is NULL, stream will be unbuffered.
*
*       Since setbuf()'s functionality is a subset of setvbuf(), simply
*       call the latter routine to do the actual work.
*
*       NOTE: For compatibility reasons, setbuf() uses BUFSIZ as the
*       buffer size rather than _INTERNAL_BUFSIZ. The reason for this,
*       and for the two BUFSIZ constants, is to allow stdio to use larger
*       buffers without breaking (already) compiled code.
*
*Entry:
*       FILE *stream - stream to be buffered or unbuffered
*       char *buffer - buffer of size BUFSIZ or NULL
*
*Exit:
*       None.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl setbuf (
        FILE *stream,
        char *buffer
        )
{
        _ASSERTE(stream != NULL);

        if (buffer == NULL)
                setvbuf(stream, NULL, _IONBF, 0);
        else
                setvbuf(stream, buffer, _IOFBF, BUFSIZ);

}
