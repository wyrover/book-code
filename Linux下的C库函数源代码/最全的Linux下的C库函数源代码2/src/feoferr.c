/***
*feoferr.c - defines feof() and ferror()
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines feof() (test for end-of-file on a stream) and ferror() (test
*       for error on a stream).
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <internal.h>

/* remove macro definitions for feof() and ferror()
 */
#undef  feof
#undef  ferror

/***
*int feof(stream) - test for end-of-file on stream
*
*Purpose:
*       Tests whether or not the given stream is at end-of-file. Normally
*       feof() is a macro, but it must also be available as a true function
*       for ANSI.
*
*Entry:
*       FILE *stream - stream to test
*
*Exit:
*       returns nonzero (_IOEOF to be more precise) if and only if the stream
*       is at end-of-file
*
*Exceptions:
*
*******************************************************************************/

int __cdecl feof (
        FILE *stream
        )
{
        _VALIDATE_RETURN((stream != NULL),EINVAL, 0);
        return( ((stream)->_flag & _IOEOF) );
}


/***
*int ferror(stream) - test error indicator on stream
*
*Purpose:
*       Tests the error indicator for the given stream. Normally, feof() is
*       a macro, but it must also be available as a true function for ANSI.
*
*Entry:
*       FILE *stream - stream to test
*
*Exit:
*       returns nonzero (_IOERR to be more precise) if and only if the error
*       indicator for the stream is set.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl ferror (
        FILE *stream
        )
{
        _VALIDATE_RETURN((stream != NULL),EINVAL, 0);
        return( ((stream)->_flag & _IOERR) );
}
