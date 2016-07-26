/***
*ungetc.c - unget a character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines ungetc() - pushes a character back onto an input stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>


/***
*int ungetc(ch, stream) - put a character back onto a stream
*
*Purpose:
*       Guaranteed one char pushback on a stream as long as open for reading.
*       More than one char pushback in a row is not guaranteed, and will fail
*       if it follows an ungetc which pushed the first char in buffer. Failure
*       causes return of EOF.
*
*Entry:
*       char ch - character to push back
*       FILE *stream - stream to push character onto
*
*Exit:
*       returns ch
*       returns EOF if tried to push EOF, stream not opened for reading or
*       or if we have already ungetc'd back to beginning of buffer.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl ungetc (
        REG2 int ch,
        REG1 FILE *stream
        )
{
        int retval;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, EOF);

        _lock_str(stream);

        __try {
                retval = _ungetc_nolock (ch, stream);
        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}


#include <ungetc_nolock.inl>
