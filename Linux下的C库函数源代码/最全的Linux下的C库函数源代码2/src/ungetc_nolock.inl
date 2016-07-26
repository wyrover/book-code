/***
*ungetc_nolock.inl - unget a character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _ungetc_nolock() - pushes a character back onto an input stream
*
*******************************************************************************/

/***
*_ungetc_nolock() -  Ungetc() core routine (locked version)
*
*Purpose:
*       Core ungetc() routine; assumes stream is already locked.
*
*       [See ungetc() above for more info.]
*
*Entry: [See ungetc()]
*
*Exit:  [See ungetc()]
*
*Exceptions:
*
*******************************************************************************/


int __cdecl _ungetc_nolock (
        REG2 int ch,
        FILE *str
        )

{


        REG1 FILE *stream;

        _VALIDATE_STREAM_ANSI_RETURN(str, EINVAL, EOF);

        /* Init stream pointer and file descriptor */
        stream = str;

        /* Stream must be open for read and can NOT be currently in write mode.
           Also, ungetc() character cannot be EOF. */

        if (
              (ch == EOF) ||
              !(
                (stream->_flag & _IOREAD) ||
                ((stream->_flag & _IORW) && !(stream->_flag & _IOWRT))
               )
           )
                return(EOF);

        /* If stream is unbuffered, get one. */
        if (stream->_base == NULL)
        {
#ifndef _SAFECRT_IMPL
            _getbuf(stream);
#else  /* _SAFECRT_IMPL */
            /* In safecrt, we assume we always have a buffer */
            _VALIDATE_POINTER_ERROR(NULL, EOF);
#endif  /* _SAFECRT_IMPL */
        }

        /* now we know _base != NULL; since file must be buffered */

        if (stream->_ptr == stream->_base) {
                if (stream->_cnt)
                        /* my back is against the wall; i've already done
                         * ungetc, and there's no room for this one
                         */
                        return(EOF);

                stream->_ptr++;
        }

        if (stream->_flag & _IOSTRG) {
            /* If stream opened by sscanf do not modify buffer */
                if (*--stream->_ptr != (char)ch) {
                        ++stream->_ptr;
                        return(EOF);
                }
        } else
                *--stream->_ptr = (char)ch;

        stream->_cnt++;
        stream->_flag &= ~_IOEOF;
        stream->_flag |= _IOREAD;       /* may already be set */

        return(0xff & ch);
}
