/***
*spawnlpe.c - spawn a child process with environ and search along PATH
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _spawnlpe() - spawn a child process with environ/PATH search
*
*******************************************************************************/

#include <cruntime.h>
#include <stddef.h>
#include <process.h>
#include <stdarg.h>
#include <internal.h>
#include <malloc.h>
#include <tchar.h>
#include <dbgint.h>

/***
*int _spawnlpe(modeflag, filename, arglist) - spawn a child process
*
*Purpose:
*       Spawns a child process.
*       formats the parameters and calls _spawnvpe to do the work of searching
*       the PATH environment variable and calling _spawnve.  The NULL
*       environment pointer indicates that the new process will inherit the
*       parents process's environment.  NOTE - at least one argument must be
*       present.  This argument is always, by convention, the name of the file
*       being spawned.
*
*Entry:
*       int modeflag   - defines what mode of spawn (WAIT, NOWAIT, OVERLAY)
*                        only WAIT and OVERLAY currently supported
*       _TSCHAR *pathname - file to spawn
*       _TSCHAR *arglist  - list of arguments (environ at end)
*       call as _spawnlpe(modeflag, path, arg0, arg1, ..., argn, NULL, envp);
*
*Exit:
*       returns exit code of spawned process
*       returns -1 if fails
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _tspawnlpe (
        int modeflag,
        const _TSCHAR *filename,
        const _TSCHAR *arglist,
        ...
        )
{
#ifdef _M_IX86

        REG1 const _TSCHAR **argp;

        /* validation section */
        _VALIDATE_RETURN(filename != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*filename != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        argp = &arglist;
        while (*argp++)
                ;

        return(_tspawnvpe(modeflag,filename,&arglist,(_TSCHAR **)*argp));

#else  /* _M_IX86 */

        va_list vargs;
        _TSCHAR * argbuf[64];
        _TSCHAR ** argv;
        _TSCHAR ** envp;
        intptr_t result;

        /* validation section */
        _VALIDATE_RETURN(filename != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*filename != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        va_start(vargs, arglist);
#ifdef WPRFLAG
        argv = _wcapture_argv(&vargs, arglist, argbuf, 64);
#else  /* WPRFLAG */
        argv = _capture_argv(&vargs, arglist, argbuf, 64);
#endif  /* WPRFLAG */
        envp = va_arg(vargs, _TSCHAR **);
        va_end(vargs);

        result = _tspawnvpe(modeflag,filename,argv,envp);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
