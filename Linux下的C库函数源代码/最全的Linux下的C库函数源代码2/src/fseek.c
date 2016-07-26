/***
*fseek.c - reposition file pointer on a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fseek() - move the file pointer to new place in file
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <dbgint.h>
#include <msdos.h>
#include <errno.h>
#include <malloc.h>
#include <io.h>
#include <stddef.h>
#include <internal.h>
#include <mtdll.h>

/***
*int fseek(stream, offset, whence) - reposition file pointer
*
*Purpose:
*
*       Reposition file pointer to the desired location.  The new location
*       is calculated as follows:
*                                { whence=0, beginning of file }
*               <offset> bytes + { whence=1, current position  }
*                                { whence=2, end of file       }
*
*       Be careful to coordinate with buffering.
*
*                       - - - - - - - - - - - - -
*
*       [NOTE: We used to bend over backwards to try and preserve the current
*       buffer and maintain disk block alignment.  This ended up making our
*       code big and slow and complicated, and slowed us down quite a bit.
*       Some of the things pertinent to the old implimentation:
*
*       (1) Read-only: We only did the special code path if the file was
*       opened read-only (_IOREAD).  If the file was writable, we didn't
*       try to optimize.
*
*       (2) Buffering:  We'd assign a buffer, if necessary, since the
*       later code might need it (i.e., call _getbuf).
*
*       (3) Ungetc: Fseek had to be careful NOT to save the buffer if
*       an ungetc had ever been done on the buffer (flag _IOUNGETC).
*
*       (4) Control ^Z: Fseek had to deal with ^Z after reading a
*       new buffer's worth of data (flag _IOCTRLZ).
*
*       (5) Seek-to-end-and-back: To determine if the new seek was within
*       the current buffer, we had to 'normalize' the desired location.
*       This means that we sometimes had to seek to the end of the file
*       and back to determine what the 0-relative offset was.  Two extra
*       lseek() calls hurt performance.
*
*       (6) CR/LF accounting - When trying to seek within a buffer that
*       is in text mode, we had to go account for CR/LF expansion.  This
*       required us to look at every character up to the new offset and
*       see if it was '\n' or not.  In addition, we had to check the
*       FCRLF flag to see if the new buffer started with '\n'.
*
*       Again, all of these notes are for the OLD implimentation just to
*       remind folks of some of the issues involving seeking within a buffer
*       and maintaining buffer alignment.  As an aside, I think this may have
*       been a big win in the 'old days' on floppy-based systems but on newer
*       fast hard disks, the extra code/complexity overwhelmed any gain.
*
*                       - - - - - - - - - - - - -
*
*Entry:
*       FILE *stream - file to reposition file pointer on
*       long offset - offset to seek to
*       int whence - origin offset is measured from (0=beg, 1=current pos,
*                    2=end)
*
*Exit:
*       returns 0 if succeeds
*       returns -1 and sets errno if fails
*       fields of FILE struct will be changed
*
*Exceptions:
*
*******************************************************************************/


int __cdecl fseek (
        FILE *stream,
        long offset,
        int whence
        )
{
        int retval;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, -1);
        _VALIDATE_RETURN( ((whence == SEEK_SET) || (whence == SEEK_CUR) || (whence == SEEK_END)), EINVAL, -1);

        _lock_str(stream);

        __try {
                retval = _fseek_nolock (stream, offset, whence);
        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}


/***
*_fseek_nolock() - Core fseek() routine (stream is locked)
*
*Purpose:
*       Core fseek() routine; assumes that caller has the stream locked.
*
*       [See fseek() for more info.]
*
*Entry: [See fseek()]
*
*Exit:  [See fseek()]
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fseek_nolock (


        FILE *str,
        long offset,
        int whence
        )
{


        REG1 FILE *stream;


        /* Init stream pointer */
        stream = str;

        if ( !inuse(stream)) {
                errno=EINVAL;
                return(-1);
        }

        /* Clear EOF flag */

        stream->_flag &= ~_IOEOF;

        /* If seeking relative to current location, then convert to
           a seek relative to beginning of file.  This accounts for
           buffering, etc. by letting fseek() tell us where we are. */

        if (whence == SEEK_CUR) {
                offset += _ftell_nolock(stream);
                whence = SEEK_SET;
        }

        /* Flush buffer as necessary */

        _flush(stream);

        /* If file opened for read/write, clear flags since we don't know
           what the user is going to do next. If the file was opened for
           read access only, decrease _bufsiz so that the next _filbuf
           won't cost quite so much */

        if (stream->_flag & _IORW)
                stream->_flag &= ~(_IOWRT|_IOREAD);
        else if ( (stream->_flag & _IOREAD) && (stream->_flag & _IOMYBUF) &&
                  !(stream->_flag & _IOSETVBUF) )
                stream->_bufsiz = _SMALL_BUFSIZ;

        /* Seek to the desired locale and return. */

        return(_lseek(_fileno(stream), offset, whence) == -1L ? -1 : 0);
}
