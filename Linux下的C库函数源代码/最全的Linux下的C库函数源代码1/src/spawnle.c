/***
*spawnle.c - spawn a child process with given environment
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _spawnle() - spawn a child process with given environ
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
*int _spawnle(modeflag, pathname, arglist) - spawn a child process with env.
*
*Purpose:
*       Spawns a child process with given parameters and environment.
*       formats the parameters and calls _spawnve to do the actual work.
*       NOTE - at least one argument must be present.  This argument is always,
*       by convention, the name of the file being spawned.
*
*Entry:
*       int modeflag   - mode of spawn (WAIT, NOWAIT, OVERLAY)
*                        only WAIT, and OVERLAY currently implemented
*       _TSCHAR *pathname - name of file to spawn
*       _TSCHAR *arglist  - argument list, environment is at the end
*       call as _spawnle(modeflag, path, arg0, arg1, ..., argn, NULL, envp);
*
*Exit:
*       returns exit code of spawned process
*       if fails, return -1
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _tspawnle (
        int modeflag,
        const _TSCHAR *pathname,
        const _TSCHAR *arglist,
        ...
        )
{
#ifdef _M_IX86

        REG1 const _TSCHAR **argp;

        /* validation section */
        _VALIDATE_RETURN(pathname != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*pathname != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        /* walk the arglist until the terminating NULL pointer is found.  The
         * next location holds the environment table pointer.
         */

        argp = &arglist;
        while (*argp++)
                ;

        return(_tspawnve(modeflag,pathname,&arglist,(_TSCHAR **)*argp));

#else  /* _M_IX86 */

        va_list vargs;
        _TSCHAR * argbuf[64];
        _TSCHAR ** argv;
        _TSCHAR ** envp;
        intptr_t result;

        /* validation section */
        _VALIDATE_RETURN(pathname != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*pathname != _T('\0'), EINVAL, -1);
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

        result = _tspawnve(modeflag,pathname,argv,envp);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
