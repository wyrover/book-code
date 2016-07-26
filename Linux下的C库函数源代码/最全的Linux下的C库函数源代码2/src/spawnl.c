/***
*spawnl.c - spawn a child process
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _spawnl() - spawn a child process
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <stdarg.h>
#include <internal.h>
#include <malloc.h>
#include <tchar.h>
#include <dbgint.h>

/***
*int _spawnl(modeflag, pathname, arglist) - spawn a child process
*
*Purpose:
*       Spawns a child process.
*       formats the parameters and calls spawnve to do the actual work. The
*       new process will inherit the parent's environment. NOTE - at least
*       one argument must be present.  This argument is always, by convention,
*       the name of the file being spawned.
*
*Entry:
*       int modeflag   - defines which mode of spawn (WAIT, NOWAIT, or OVERLAY)
*                        only WAIT and OVERLAY are currently implemented
*       _TSCHAR *pathname - file to be spawned
*       _TSCHAR *arglist  - list of argument
*       call as _spawnl(modeflag, path, arg0, arg1, ..., argn, NULL);
*
*Exit:
*       returns exit code of child process
*       returns -1 if fails
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _tspawnl (
        int modeflag,
        const _TSCHAR *pathname,
        const _TSCHAR *arglist,
        ...
        )
{
#ifdef _M_IX86

        /* validation section */
        _VALIDATE_RETURN(pathname != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*pathname != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        return(_tspawnve(modeflag,pathname,&arglist,NULL));

#else  /* _M_IX86 */

        va_list vargs;
        _TSCHAR * argbuf[64];
        _TSCHAR ** argv;
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
        va_end(vargs);

        result = _tspawnve(modeflag,pathname,argv,NULL);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
