/***
*telli64.c - find file position
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _telli64 - find file position
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <stdio.h>

/***
*__int64 _telli64(filedes) - find file position
*
*Purpose:
*       Gets the current position of the file pointer (no adjustment
*       for buffering).
*
*Entry:
*       int filedes - file handle of file
*
*Exit:
*       returns file position or -1i64 (sets errno) if bad file descriptor or
*       pipe
*
*Exceptions:
*
*******************************************************************************/

__int64 __cdecl _telli64 (
        int filedes
        )
{
        return( _lseeki64( filedes, 0i64, SEEK_CUR ) );
}
