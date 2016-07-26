/***
*spawnv.c - spawn a child process
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _spawnv() - spawn a child process
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <tchar.h>
#include <dbgint.h>
#include <internal.h>

/***
*int _spawnv(modeflag, pathname, argv) - spawn a child process
*
*Purpose:
*       Spawns a child process.
*       formats the parameters and calls _spawnve to do the actual work.  The
*       NULL environment pointer indicates that the new process will inherit
*       the parents process's environment.  NOTE - at least one argument must
*       be present.  This argument is always, by convention, the name of the
*       file being spawned.
*
*Entry:
*       int modeflag   - mode to spawn (WAIT, NOWAIT, or OVERLAY)
*                        only WAIT and OVERLAY currently implemented
*       _TSCHAR *pathname - file to spawn
*       _TSCHAR **argv    - vector of arguments
*
*Exit:
*       returns exit code of child process
*       if fails, returns -1
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _tspawnv (
        int modeflag,
        const _TSCHAR *pathname,
        const _TSCHAR * const *argv
        )
{
        /* validation section */
        _VALIDATE_RETURN(pathname != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*pathname != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(argv != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*argv != NULL, EINVAL, -1);
        _VALIDATE_RETURN(**argv != _T('\0'), EINVAL, -1);

        return(_tspawnve(modeflag,pathname,argv,NULL));
}
