/***
*setvbuf.c - set buffer size for a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines setvbuf() - set the buffering mode and size for a stream.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <malloc.h>
#include <internal.h>
#include <mtdll.h>
#include <limits.h>
#include <dbgint.h>

/***
*int setvbuf(stream, buffer, type, size) - set buffering for a file
*
*Purpose:
*       Controls buffering and buffer size for the specified stream.  The
*       array pointed to by buf is used as a buffer, unless NULL, in which
*       case we'll allocate a buffer automatically. type specifies the type
*       of buffering: _IONBF = no buffer, _IOFBF = buffered, _IOLBF = same
*       as _IOFBF.
*
*Entry:
*       FILE *stream - stream to control buffer on
*       char *buffer - pointer to buffer to use (NULL means auto allocate)
*       int type     - type of buffering (_IONBF, _IOFBF or _IOLBF)
*       size_t size  - size of buffer
*
*Exit:
*       return 0 if successful
*       returns non-zero if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl setvbuf (
        FILE *str,
        char *buffer,
        int type,
        size_t size
        )
{
        REG1 FILE *stream;
        int retval=0;   /* assume good return */

        _VALIDATE_RETURN( (str != NULL), EINVAL, -1 );

        /*
         * (1) Make sure type is one of the three legal values.
         * (2) If we are buffering, make sure size is between 2 and INT_MAX.
         */
        _VALIDATE_RETURN( (type == _IONBF) || (type == _IOFBF) || (type == _IOLBF), EINVAL, -1 );

        if ((type == _IOFBF) || (type == _IOLBF))
        {
            _VALIDATE_RETURN( ((2 <= size) && (size <= INT_MAX)), EINVAL, -1 );
        }

        /*
         * force size to be even by masking down to the nearest multiple
         * of 2
         */
        size &= (size_t)~1;

        /*
         * Init stream pointers
         */
        stream = str;

        /*
         * Lock the file
         */
        _lock_str(stream);
        __try {

        /*
         * Flush the current buffer and free it, if it is ours.
         */
        _flush(stream);
        _freebuf(stream);

        /*
         * Clear a bunch of bits in stream->_flag (all bits related to
         * buffering and those which used to be in stream2->_flag2). Most
         * of these should never be set when setvbuf() is called, but it
         * doesn't cost anything to be safe.
         */
        stream->_flag &= ~(_IOMYBUF | _IOYOURBUF | _IONBF |
                           _IOSETVBUF | _IOFEOF | _IOFLRTN | _IOCTRLZ);

        /*
         * CASE 1: No Buffering.
         */
        if (type & _IONBF) {
                stream->_flag |= _IONBF;
                buffer = (char *)&(stream->_charbuf);
                size = 2;
        }

        /*
         * NOTE: Cases 2 and 3 (below) cover type == _IOFBF or type == _IOLBF
         * Line buffering is treated as the same as full buffering, so the
         * _IOLBF bit in stream->_flag is never set. Finally, since _IOFBF is
         * defined to be 0, full buffering is simply assumed whenever _IONBF
         * is not set.
         */

        /*
         * CASE 2: Default Buffering -- Allocate a buffer for the user.
         */
        else if ( buffer == NULL ) {
                if ( (buffer = _malloc_crt(size)) == NULL ) {
#ifndef CRTDLL
                        /*
                         * force library pre-termination procedure (placed here
                         * because the code path should almost never be hit)
                         */
                        _cflush++;
#endif  /* CRTDLL */
                        retval = -1;
                        goto done;
                }
                stream->_flag |= _IOMYBUF | _IOSETVBUF;
        }

        /*
         * CASE 3: User Buffering -- Use the buffer supplied by the user.
         */
        else {
                stream->_flag |= _IOYOURBUF | _IOSETVBUF;
        }

        /*
         * Common return for all cases.
         */
        stream->_bufsiz = (int)size;
        stream->_ptr = stream->_base = buffer;
        stream->_cnt = 0;
done:

        ; }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}
