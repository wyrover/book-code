/***
*getw.c - read a binary word from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getw() - gets a binary integer from a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int _getw(stream) - read an int from a stream
*
*Purpose:
*       get n bytes (n=sizeof(int)); OR them together in proper order; high
*       byte first. check for EOF between getc's.
*       this routine should be machine independent.
*
*Entry:
*       FILE *stream - stream to read integer from
*
*Exit:
*       returns the int read from the stream
*       returns EOF if fails (but this is a legit int value, so
*       should test feof() or ferror().
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _getw (
        FILE *str
        )
{
        REG1 FILE *stream;
        REG2 int bytecount = sizeof(int);
        int word;
        char *byteptr = (char *)&word;
        int retval;

        _VALIDATE_RETURN((str != NULL), EINVAL, EOF);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        while (bytecount--)
            *byteptr++ = (char)_getc_nolock(stream);
        retval = ((feof(stream) || ferror(stream)) ? EOF : word);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}
