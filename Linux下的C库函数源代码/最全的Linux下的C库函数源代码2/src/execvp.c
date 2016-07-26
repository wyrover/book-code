/***
*execvp.c - execute a file and search along PATH
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _execvp() - execute a file and search along PATH
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>

/***
*int _execvp(filename, argvector) - execute file; search along PATH
*
*Purpose:
*       Execute the given file with given path and current environ.
*       try to execute the file. start with the name itself (directory '.'),
*       and if that doesn't work start prepending pathnames from the
*       environment until one works or we run out. if the file is a pathname,
*       don't go to the environment to get alternate paths. if errno comes
*       back ENOEXEC, try it as a shell command file with up to MAXARGS-2
*       arguments from the original vector. if a needed text file is busy,
*       wait a little while and try again before despairing completely
*       Actually calls _execvpe() to do all the work.
*
*Entry:
*       _TSCHAR *filename        - file to execute
*       _TSCHAR **argvector - vector of arguments
*
*Exit:
*       destroys the calling process (hopefully)
*       if fails, returns -1
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _texecvp (
        REG3 const _TSCHAR *filename,
        const _TSCHAR * const *argvector
        )
{
        return _texecvpe( filename, argvector, NULL );
}
