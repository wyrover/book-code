/***
*_filbuf.c - fill buffer and get character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _filbuf() - fill buffer and read first character, allocate
*       buffer if there is none.  Used from getc().
*       defines _filwbuf() - fill buffer and read first wide character, allocate
*       buffer if there is none.  Used from getwc().
*
*Note:
*       this file is included in safecrt.lib build directly, plese refer
*       to safecrt_filwbuf.c
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <io.h>
#include <dbgint.h>
#include <malloc.h>
#include <internal.h>
#include <msdos.h>
#include <wchar.h>
#include <mtdll.h>
#include <tchar.h>

#ifndef _UNICODE

/***
*int _filbuf(stream) - fill buffer and get first character
*
*Purpose:
*       get a buffer if the file doesn't have one, read into it, return first
*       char. try to get a buffer, if a user buffer is not assigned. called
*       only from getc; intended for use only within library. assume no input
*       stream is to remain unbuffered when memory is available unless it is
*       marked _IONBF. at worst, give it a single char buffer. the need for a
*       buffer, no matter how small, becomes evident when we consider the
*       ungetc's necessary in scanf
*
*       [NOTE: Multi-thread - _filbuf() assumes that the caller has aquired
*       the stream lock, if needed.]
*
*Entry:
*       FILE *stream - stream to read from
*
*Exit:
*       returns first character from buffer (next character to be read)
*       returns EOF if the FILE is actually a string, or not open for reading,
*       or if open for writing or if no more chars to read.
*       all fields in FILE structure may be changed except _file.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _filbuf (
        FILE *str
        )

#else  /* _UNICODE */

/***
*int _filwbuf(stream) - fill buffer and get first wide character
*
*Purpose:
*       get a buffer if the file doesn't have one, read into it, return first
*       char. try to get a buffer, if a user buffer is not assigned. called
*       only from getc; intended for use only within library. assume no input
*       stream is to remain unbuffered when memory is available unless it is
*       marked _IONBF. at worst, give it a single char buffer. the need for a
*       buffer, no matter how small, becomes evident when we consider the
*       ungetc's necessary in scanf
*
*       [NOTE: Multi-thread - _filwbuf() assumes that the caller has aquired
*       the stream lock, if needed.]
*
*Entry:
*       FILE *stream - stream to read from
*
*Exit:
*       returns first wide character from buffer (next character to be read)
*       returns WEOF if the FILE is actually a string, or not open for reading,
*       or if open for writing or if no more chars to read.
*       all fields in FILE structure may be changed except _file.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _filwbuf (
        FILE *str
        )

#endif  /* _UNICODE */

{

        REG1 FILE *stream=NULL;

                /* In safecrt, we assume we always have a buffer */
        _VALIDATE_RETURN(str != NULL, EINVAL, _TEOF);

        /* Init pointer to _iob2 entry. */
        stream = str;

        if (!inuse(stream) || stream->_flag & _IOSTRG)
                return(_TEOF);

        if (stream->_flag & _IOWRT) {
                stream->_flag |= _IOERR;
                return(_TEOF);
        }

        stream->_flag |= _IOREAD;

        /* Get a buffer, if necessary. */

        if (!anybuf(stream))
        {
#ifndef _SAFECRT_IMPL
            _getbuf(stream);
#else  /* _SAFECRT_IMPL */
            /* In safecrt, we assume we always have a buffer */
            _VALIDATE_RETURN(FALSE, EINVAL, _TEOF);
#endif  /* _SAFECRT_IMPL */
        }
        else
        {
            stream->_ptr = stream->_base;
        }

        stream->_cnt = _read(_fileno(stream), stream->_base, stream->_bufsiz);

#ifndef _UNICODE
        if ((stream->_cnt == 0) || (stream->_cnt == -1)) {
#else  /* _UNICODE */
        if ((stream->_cnt == 0) || (stream->_cnt == 1) || stream->_cnt == -1) {
#endif  /* _UNICODE */
                stream->_flag |= stream->_cnt ? _IOERR : _IOEOF;
                stream->_cnt = 0;
                return(_TEOF);
        }

        if (  !(stream->_flag & (_IOWRT|_IORW)) &&
              ((_osfile_safe(_fileno(stream)) & (FTEXT|FEOFLAG)) ==
                (FTEXT|FEOFLAG)) )
                stream->_flag |= _IOCTRLZ;
        /* Check for small _bufsiz (_SMALL_BUFSIZ). If it is small and
           if it is our buffer, then this must be the first _filbuf after
           an fseek on a read-access-only stream. Restore _bufsiz to its
           larger value (_INTERNAL_BUFSIZ) so that the next _filbuf call,
           if one is made, will fill the whole buffer. */
        if ( (stream->_bufsiz == _SMALL_BUFSIZ) && (stream->_flag &
              _IOMYBUF) && !(stream->_flag & _IOSETVBUF) )
        {
                stream->_bufsiz = _INTERNAL_BUFSIZ;
        }
#ifndef _UNICODE
        stream->_cnt--;
        return(0xff & *stream->_ptr++);
#else  /* _UNICODE */
        stream->_cnt -= sizeof(wchar_t);
        return (0xffff & *((wchar_t *)(stream->_ptr))++);
#endif  /* _UNICODE */

}
