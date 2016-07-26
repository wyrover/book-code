/***
*fputc.c - write a character to an output stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fputc() - writes a character to a stream
*       defines fputwc() - writes a wide character to a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int fputc(ch, stream) - write a character to a stream
*
*Purpose:
*       Writes a character to a stream.  Function version of putc().
*
*Entry:
*       int ch - character to write
*       FILE *stream - stream to write to
*
*Exit:
*       returns the character if successful
*       returns EOF if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fputc (
        int ch,
        FILE *str
        )
{
        int retval=0;

        _VALIDATE_RETURN((str != NULL), EINVAL, EOF);

        _lock_str(str);
        __try {

        _VALIDATE_STREAM_ANSI_SETRET(str, EINVAL, retval, EOF);
                if(retval==0)
                {
                        retval = _putc_nolock(ch,str);
                }

        }
        __finally {
            _unlock_str(str);
        }

        return(retval);
}

#undef putc

int __cdecl putc (
        int ch,
        FILE *str
        )
{
        int retval=0;

        _VALIDATE_RETURN((str != NULL), EINVAL, EOF);

        _lock_str(str);
        __try {

        _VALIDATE_STREAM_ANSI_SETRET(("Invalid ANSI I/O on unicode stream", str), EINVAL, retval, EOF);
                if(retval==0)
                {
                        retval = _putc_nolock(ch,str);
                }

        }
        __finally {
            _unlock_str(str);
        }

        return(retval);
}
