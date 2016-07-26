/***
*execlpe.c - execute a file with environ, search along path
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _execlpe() - execute a file with environ and search along PATH
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <process.h>
#include <stdarg.h>
#include <internal.h>
#include <tchar.h>
#include <dbgint.h>

/***
*int _execlpe(filename, arglist) - execute a file with environ
*
*Purpose:
*       Executes the given file with the parameters and the environment
*       which is passed after the parameters.  Searches along the PATH
*       for the file (done by execvp).
*
*Entry:
*       _TSCHAR *filename - file to execute
*       _TSCHAR *arglist  - argument list (environment is at the end)
*       call as _execlpe(path, arg0, arg1, ..., argn, NULL, envp);
*
*Exit:
*       destroys the calling process (hopefully)
*       if fails, returns -1
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _texeclpe (
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

        return(_texecvpe(filename,&arglist,(_TSCHAR **)*argp));

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

        result = _texecvpe(filename,argv,envp);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
