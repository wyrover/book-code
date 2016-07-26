/***
*execle.c - execute a file with arg list and environment
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _execle() - execute a file
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
*int _execle(filename, arglist) - execute a file
*
*Purpose:
*       Execute the given file (overlays the calling process).
*       We must dig the environment vector out of the stack and pass it
*       and address of argument vector to execve.
*
*Entry:
*       _TSCHAR *filename - file to execute
*       _TSCHAR *arglist  - argument list followed by environment
*       should be called like _execle(path, arg0, arg1, ..., argn, NULL, envp);
*
*Exit:
*       destroys calling process (hopefully)
*       if fails, returns -1.
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _texecle (
        const _TSCHAR *filename,
        const _TSCHAR *arglist,
        ...
        )
{
#ifdef _M_IX86

        REG1 const _TSCHAR **e_search = &arglist;

        /* validation section */
        _VALIDATE_RETURN(filename != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*filename != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        while (*e_search++)
                ;

        return(_texecve(filename,&arglist,(_TSCHAR **)*e_search));

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

        result = _texecve(filename,argv,envp);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
