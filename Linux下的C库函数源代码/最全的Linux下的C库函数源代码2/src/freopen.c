/***
*freopen.c - close a stream and assign it to a new file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines freopen() - close and reopen file, typically used to redirect
*       stdin/out/err/prn/aux.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <share.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>
#include <tchar.h>

errno_t __cdecl _tfreopen_helper (
        FILE ** pfile,
        const _TSCHAR *filename,
        const _TSCHAR *mode,
        FILE *str,
        int shflag
        )
{
        REG1 FILE *stream;

        _VALIDATE_RETURN_ERRCODE( (pfile != NULL), EINVAL);
        *pfile = NULL;
        _VALIDATE_RETURN_ERRCODE( (filename != NULL), EINVAL);
        _VALIDATE_RETURN_ERRCODE( (mode != NULL), EINVAL);
        _VALIDATE_RETURN_ERRCODE( (str != NULL), EINVAL);

        /* We deliberately don't hard-validate for emptry strings here. All other invalid
        path strings are treated as runtime errors by the inner code in _open and openfile.
        This is also the appropriate treatment here. Since fopen is the primary access point
        for file strings it might be subjected to direct user input and thus must be robust to
        that rather than aborting. The CRT and OS do not provide any other path validator (because
        WIN32 doesn't allow such things to exist in full generality).
        */
        if(*filename==_T('\0'))
        {
            errno=EINVAL;
            return errno;
        }

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        /* If the stream is in use, try to close it. Ignore possible
         * error (ANSI 4.9.5.4). */
        if ( inuse(stream) )
        {
            _fclose_nolock(stream);
        }

        stream->_ptr = stream->_base = NULL;
        stream->_cnt = stream->_flag = 0;
#ifdef _UNICODE
        *pfile = _wopenfile(filename,mode,shflag,stream);
#else  /* _UNICODE */
        *pfile = _openfile(filename,mode,shflag,stream);
#endif  /* _UNICODE */

        }
        __finally {
                _unlock_str(stream);
        }

        if(*pfile)
            return 0;

        return errno;
}

/***
*FILE *freopen(filename, mode, stream) - reopen stream as new file
*
*Purpose:
*       Closes the file associated with stream and assigns stream to a new
*       file with current mode.  Usually used to redirect a standard file
*       handle.
*
*Entry:
*       char *filename - new file to open
*       char *mode - new file mode, as in fopen()
*       FILE *stream - stream to close and reassign
*
*Exit:
*       returns stream if successful
*       return NULL if fails
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _tfreopen (
        const _TSCHAR *filename,
        const _TSCHAR *mode,
        FILE *str
        )
{
    FILE * fp = NULL;

    _tfreopen_helper(&fp,filename,mode,str,_SH_DENYNO);

    return fp;
}

/***
*errno_t freopen(pfile,filename, mode, stream) - reopen stream as new file
*
*Purpose:
*       Closes the file associated with stream and assigns stream to a new
*       file with current mode.  Usually used to redirect a standard file
*       handle.This is the secure version fopen - it opens the file
*       in _SH_DENYRW share mode.

*
*Entry:
*       FILE **pfile - Pointer to return the FILE handle into.
*       char *filename - new file to open
*       char *mode - new file mode, as in fopen()
*       FILE *stream - stream to close and reassign
*
*Exit:
*       returns 0 on success & sets pfile
*       returns errno_t on failure.
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tfreopen_s (
        FILE ** pfile,
        const _TSCHAR *filename,
        const _TSCHAR *mode,
        FILE *str
        )
{
    return _tfreopen_helper(pfile,filename,mode,str,_SH_SECURE);
}
