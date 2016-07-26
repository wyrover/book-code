/***
*fileno.c - defines _fileno()
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines fileno() - return the file handle for the specified stream
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdio.h>

/* remove macro definition for fileno()
 */
#undef  _fileno

/***
*int _fileno(stream) - return the file handle for stream
*
*Purpose:
*       Returns the file handle for the given stream is. Normally fileno()
*       is a macro, but it is also available as a true function (for
*       consistency with ANSI, though it is not required).
*
*Entry:
*       FILE *stream - stream to fetch handle for
*
*Exit:
*       returns the file handle for the given stream
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fileno (
        FILE *stream
        )
{
        _VALIDATE_RETURN((stream != NULL), EINVAL, -1);
        return( stream->_file );
}
