/***
*execlp.c - execute a file (search along PATH)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _execlp() - execute a file and search along PATH
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
*int _execlp(filename, arglist) - execute a file, search along PATH
*
*Purpose:
*       Execute the given file with the given arguments; search along PATH
*       for the file. We pass the arguments to execvp where several paths
*       will be tried until one works.
*
*Entry:
*       _TSCHAR *filename - file to execute
*       _TSCHAR *arglist  - argument list
*       call as _execlp(path, arg0, arg1, ..., argn, NULL);
*
*Exit:
*       destroys calling process (hopefully)
*       returns -1 if fails.
*
*Exceptions:
*
*******************************************************************************/

intptr_t __cdecl _texeclp (
        const _TSCHAR *filename,
        const _TSCHAR *arglist,
        ...
        )
{
#ifdef _M_IX86

        /* validation section */
        _VALIDATE_RETURN(filename != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*filename != _T('\0'), EINVAL, -1);
        _VALIDATE_RETURN(arglist != NULL, EINVAL, -1);
        _VALIDATE_RETURN(*arglist != _T('\0'), EINVAL, -1);

        return(_texecvp(filename,&arglist));

#else  /* _M_IX86 */

        va_list vargs;
        _TSCHAR * argbuf[64];
        _TSCHAR ** argv;
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
        va_end(vargs);

        result = _texecvp(filename,argbuf);
        if (argv && argv != argbuf)
            _free_crt(argv);
        return result;

#endif  /* _M_IX86 */
}
