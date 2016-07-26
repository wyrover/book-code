/***
*fgetpos.c - Contains the fgetpos runtime
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Get file position (in an internal format).
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <internal.h>

/***
*int fgetpos(stream,pos) - Get file position (internal format)
*
*Purpose:
*       Fgetpos gets the current file position for the file identified by
*       [stream].  The file position is returned in the object pointed to
*       by [pos] and is in internal format; that is, the user is not supposed
*       to interpret the value but simply use it in the fsetpos call.  Our
*       implementation simply uses fseek/ftell.
*
*Entry:
*       FILE *stream = pointer to a file stream value
*       fpos_t *pos = pointer to a file position value
*
*Exit:
*       Successful fgetpos call returns 0.
*       Unsuccessful fgetpos call returns non-zero (!0) value and sets
*       ERRNO (this is done by ftell and passed back by fgetpos).
*
*Exceptions:
*       None.
*
*******************************************************************************/

int __cdecl fgetpos (
        FILE *stream,
        fpos_t *pos
        )
{

        _VALIDATE_RETURN((stream != NULL), EINVAL, -1);
        _VALIDATE_RETURN((pos != NULL), EINVAL, -1);

        if ( (*pos = _ftelli64(stream)) != -1i64 )
                return(0);
        else
                return(-1);
}
