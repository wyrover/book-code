/***
*fwrite.c - read from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Write to the specified stream from the user's buffer.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <mtdll.h>
#include <io.h>
#include <string.h>
#include <file2.h>
#include <internal.h>
#include <dbgint.h>

/***
*size_t fwrite(void *buffer, size_t size, size_t count, FILE *stream) -
*       write to the specified stream from the specified buffer.
*
*Purpose:
*       Write 'count' items of size 'size' to the specified stream from
*       the specified buffer. Return when 'count' items have been written
*       or no more items can be written to the stream.
*
*Entry:
*       buffer  - pointer to user's buffer
*       size    - size of the item to write
*       count   - number of items to write
*       stream  - stream to write to
*
*Exit:
*       Returns the number of (whole) items that were written to the stream.
*       This may be less than 'count' if an error or eof occurred. In this
*       case, ferror() or feof() should be used to distinguish between the
*       two conditions.
*
*Notes:
*       fwrite will attempt to buffer the stream (side effect of the _flsbuf
*       call) if necessary.
*
*       No more than 0xFFFE bytes may be written out at a time by a call to
*       write(). Further, write() does not handle huge buffers. Therefore,
*       in large data models, the write request is broken down into chunks
*       that do not violate these considerations. Each of these chunks is
*       processed much like an fwrite() call in a small data model (by a
*       call to _nfwrite()).
*
*       This code depends on _iob[] being a near array.
*
*       MTHREAD/DLL - Handled in just two layers since it is small data
*       model. The outer layer, fwrite(), handles stream locking/unlocking
*       and calls _fwrite_nolock() to do the work. _fwrite_nolock() is the same as
*       the single-thread, small data model version of fwrite().
*
*******************************************************************************/


/* define locking/unlocking version */
size_t __cdecl fwrite (
        const void *buffer,
        size_t size,
        size_t count,
        FILE *stream
        )
{
        size_t retval;

        if(size * count == 0)
            return 0;

        _VALIDATE_RETURN_ERRCODE( (stream != NULL) , 0);
        _VALIDATE_RETURN_ERRCODE( (buffer != NULL) , 0);

        _lock_str(stream);                      /* lock stream */

        __try {
                /* do the read */
                retval = _fwrite_nolock(buffer, size, count, stream);
        }
        __finally {
                _unlock_str(stream);            /* unlock stream */
        }

        return retval;
}

/* define the normal version */
size_t __cdecl _fwrite_nolock (
        const void *buffer,
        size_t size,
        size_t num,
        FILE *stream
        )
{
        const char *data;               /* point to where data comes from next */
        size_t total;                   /* total bytes to write */
        size_t count;                   /* num bytes left to write */
        unsigned bufsize;               /* size of stream buffer */
        unsigned nbytes;                /* number of bytes to write now */
        unsigned nactuallywritten;      /* number of bytes actually written by write() call*/
        unsigned nwritten;              /* number of bytes we consider written */
        int c;                          /* a temp char */

        /* initialize local vars */
        data = buffer;
        count = total = size * num;
        if (0 == count)
            return 0;


        if (anybuf(stream))
                /* already has buffer, use its size */
                bufsize = stream->_bufsiz;
        else
                /* assume will get _INTERNAL_BUFSIZ buffer */
                bufsize = _INTERNAL_BUFSIZ;

        /* here is the main loop -- we go through here until we're done */
        while (count != 0) {
                /* if the buffer is big and has room, copy data to buffer */
                if (bigbuf(stream) && stream->_cnt != 0) {
                         if(stream->_cnt < 0) {
                            _ASSERTE(("Inconsistent Stream Count. Flush between consecutive read and write", stream->_cnt >= 0));
                            stream->_flag |= _IOERR;
                            return (total - count) / size;
                        }

                        /* how much do we want? */
                        nbytes = (count < (unsigned)stream->_cnt) ? (unsigned)count : stream->_cnt;
                        memcpy(stream->_ptr, data, nbytes);

                        /* update stream and amt of data written */
                        count -= nbytes;
                        stream->_cnt -= nbytes;
                        stream->_ptr += nbytes;
                        data += nbytes;
                }
                else if (count >= bufsize) {
                        /* If we have more than bufsize chars to write, write
                           data by calling write with an integral number of
                           bufsiz blocks.  If we reach here and we have a big
                           buffer, it must be full so _flush it. */

                        if (bigbuf(stream)) {
                                if (_flush(stream)) {
                                        /* error, stream flags set -- we're out
                                           of here */
                                        return (total - count) / size;
                                }
                        }

                        /* calc chars to read -- (count/bufsize) * bufsize */
                        nbytes = ( bufsize ? (unsigned)(count - count % bufsize) :
                                   (unsigned)count );

                        nactuallywritten = _write(_fileno(stream), data, nbytes);
                        if (nactuallywritten == (unsigned)EOF) {
                                /* error -- out of here */
                                stream->_flag |= _IOERR;
                                return (total - count) / size;
                        }

                        /* VSWhidbey#326224 - _write can return more bytes than we requested due unicode
                           involved conversions in text files.
                           But we do not care about how many bytes were written as long as the number is
                           at least as big as we wanted.
                        */
                        nwritten = (nactuallywritten > nbytes ? nbytes : nactuallywritten);

                        /* update count and data to reflect write */

                        count -= nwritten;
                        data += nwritten;

                        if (nactuallywritten < nbytes) {
                                /* error -- out of here */
                                stream->_flag |= _IOERR;
                                return (total - count) / size;
                        }
                }
                else {
                        /* buffer full and not enough chars to do direct write,
                           so do a _flsbuf. */
                        c = *data;  /* _flsbuf write one char, this is it */
                        if (_flsbuf(c, stream) == EOF) {
                                /* error or eof, stream flags set by _flsbuf */
                                return (total - count) / size;
                        }

                        /* _flsbuf wrote a char -- update count */
                        ++data;
                        --count;

                        /* update buffer size */
                        bufsize = stream->_bufsiz > 0 ? stream->_bufsiz : 1;
                }
        }

        /* we finished successfully, so just return num */
        return num;
}
