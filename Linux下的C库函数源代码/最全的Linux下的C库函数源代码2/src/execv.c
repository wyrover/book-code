/***
*execv.c - execute a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _execv() - execute a file
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>
#include <dbgint.h>
#include <internal.h>

/***
*int _execv(filename, argvector) - execute a file
*
*Purpose:
*       Executes a file with given arguments.  Passes arguments to _execve and
*       uses pointer to the default environment.
*
*Entry:
*       _TSCHAR *filename        - file to execute
*       _TSCHAR **argvector - vector of arguments.
*
*Exit:
*       destroys calling process (hopefully)
*       if fails, returns -1
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _texecv (
        const _TSCHAR *filename,
        const _TSCHAR * const *argvector
        )
{
        /* validation section */
        _VALIDATE_RETURN(filename != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*filename != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(argvector != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*argvector != NULL, EINVAL, -1);
        _VALIDATE_RETURN(**argvector != _T('\0'), EINVAL, -1);

        return(_texecve(filename,argvector,NULL));
}
