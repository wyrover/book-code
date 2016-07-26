/***
*spawnvp.c - spawn a child process; search along PATH
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _spawnvp() - spawn a child process; search along PATH
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>

/***
*int _spawnvp(modeflag, filename, argv) - spawn a child process (search PATH)
*
*Purpose:
*       Spawns a child process, with search along PATH variable.
*       formats the parameters and calls _spawnve to do the actual work. The
*       NULL environment pointer indicates the new process will inherit the
*       parents process's environment.  NOTE - at least one argument must be
*       present.  This argument is always, by convention, the name of the file
*       being spawned.
*
*Entry:
*       int modeflag   - mode to spawn (WAIT, NOWAIT, or OVERLAY)
*                        only WAIT and OVERLAY currently supported
*       _TSCHAR *pathname - name of file to spawn
*       _TSCHAR **argv    - vector of arguments
*
*Exit:
*       returns exit code of child process
*       returns -1 if fails
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _tspawnvp (
        int modeflag,
        REG3 const _TSCHAR *filename,
        const _TSCHAR * const *argv
        )
{
        return _tspawnvpe(modeflag, filename, argv, NULL);
}
