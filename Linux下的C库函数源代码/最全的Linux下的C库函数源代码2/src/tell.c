/***
*tell.c - find file position
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _tell() - find file position
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>

/***
*long _tell(filedes) - find file position
*
*Purpose:
*       Gets the current position of the file pointer (no adjustment
*       for buffering).
*
*Entry:
*       int filedes - file handle of file
*
*Exit:
*       returns file position or -1L (sets errno) if bad file descriptor or
*       pipe
*
*Exceptions:
*
*******************************************************************************/

long __cdecl _tell (
        int filedes
        )
{
        return(_lseek(filedes,0L,1));
}
