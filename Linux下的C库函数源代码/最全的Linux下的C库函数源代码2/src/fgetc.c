/***
*fgetc.c - get a character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fgetc() and getc() - read  a character from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int fgetc(stream), getc(stream) - read a character from a stream
*
*Purpose:
*       reads a character from the given stream
*
*Entry:
*       FILE *stream - stream to read character from
*
*Exit:
*       returns the character read
*       returns EOF if at end of file or error occurred
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fgetc (
        REG1 FILE *stream
        )
{
        int retval=0;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, EOF);

        _lock_str(stream);
        __try {
        _VALIDATE_STREAM_ANSI_SETRET(stream, EINVAL, retval, EOF);

                if(retval==0)
                {
                        retval = _getc_nolock(stream);
                }

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

#undef getc

int __cdecl getc (
        FILE *stream
        )
{
        int retval=0;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, EOF);

        _lock_str(stream);
        __try {

        _VALIDATE_STREAM_ANSI_SETRET(stream, EINVAL, retval, EOF);
                if(retval==0)
                {
                        retval = _getc_nolock(stream);
                }

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

_CRTIMP int (__cdecl _getc_nolock)(
        FILE *stream
        )
{
    return _getc_nolock(stream);
}
