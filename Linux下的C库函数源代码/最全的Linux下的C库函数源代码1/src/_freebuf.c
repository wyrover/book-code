/***
*_freebuf.c - release a buffer from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _freebuf() - release a buffer from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <dbgint.h>
#include <internal.h>
#include <stdlib.h>

/***
*void _freebuf(stream) - release a buffer from a stream
*
*Purpose:
*       free a buffer if at all possible. free() the space if malloc'd by me.
*       forget about trying to free a user's buffer for him; it may be static
*       memory (not from malloc), so he has to take care of it. this function
*       is not intended for use outside the library.
*
*ifdef _MT
*       Multi-thread notes:
*       _freebuf() does NOT get the stream lock; it is assumed that the
*       caller has already done this.
*endif
*
*Entry:
*       FILE *stream - stream to free bufer on
*
*Exit:
*       Buffer may be freed.
*       No return value.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _freebuf (
        REG1 FILE *stream
        )
{
        _ASSERTE(stream != NULL);

        if (inuse(stream) && mbuf(stream))
        {
                _free_crt(stream->_base);

                stream->_flag &= ~(_IOMYBUF | _IOSETVBUF);
                stream->_base = stream->_ptr = NULL;
                stream->_cnt = 0;
        }
}
