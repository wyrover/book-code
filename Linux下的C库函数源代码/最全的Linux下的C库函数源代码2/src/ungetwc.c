/***
*ungetwc.c - unget a wide character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines ungetwc() - pushes a wide character back onto an input stream
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <stdlib.h>
#include <file2.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>
#include <msdos.h>
#include <errno.h>
#include <wchar.h>
#include <setlocal.h>


/***
*wint_t ungetwc(ch, stream) - put a wide character back onto a stream
*
*Purpose:
*       Guaranteed one char pushback on a stream as long as open for reading.
*       More than one char pushback in a row is not guaranteed, and will fail
*       if it follows an ungetc which pushed the first char in buffer. Failure
*       causes return of WEOF.
*
*Entry:
*       wint_t ch - wide character to push back
*       FILE *stream - stream to push character onto
*
*Exit:
*       returns ch
*       returns WEOF if tried to push WEOF, stream not opened for reading or
*       or if we have already ungetc'd back to beginning of buffer.
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl ungetwc (
        REG2 wint_t ch,
        REG1 FILE *stream
        )
{
        wint_t retval;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, EOF);

        _lock_str(stream);

        __try {
                retval = _ungetwc_nolock (ch, stream);
        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}


#include <ungetwc_nolock.inl>

