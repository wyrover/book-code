/***
*putw.c - put a binary int to output stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _putw() - puts a binary int to an output stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int _putw(word, stream) - write a binary int to an output stream
*
*Purpose:
*       Writes sizeof(int) bytes to the output stream, high byte first.
*       This routine should be machine independent.
*
*Entry:
*       int word - integer to write
*       FILE *stream - stream to write to
*
*Exit:
*       returns the word put to the stream
*       returns EOF if error, but this is a legit int value, so should
*       test with feof() or ferror().
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _putw (
        int word,
        FILE *str
        )
{
        REG1 FILE *stream;
        REG3 int bytecount = sizeof(int);
        REG2 char *byteptr = (char *)&word;
        int retval;

        _VALIDATE_RETURN((str != NULL), EINVAL, EOF);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        while (bytecount--)
        {
            _putc_nolock(*byteptr,stream);
            ++byteptr;
        }
        retval = (ferror(stream) ? EOF : word);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}
