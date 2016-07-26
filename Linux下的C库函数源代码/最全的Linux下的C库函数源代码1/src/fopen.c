/***
*fopen.c - open a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fopen() and _fsopen() - open a file as a stream and open a file
*       with a specified sharing mode as a stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <share.h>
#include <dbgint.h>
#include <internal.h>
#include <mtdll.h>
#include <file2.h>
#include <tchar.h>
#include <errno.h>

/***
*FILE *_fsopen(file, mode, shflag) - open a file
*
*Purpose:
*       Opens the file specified as a stream.  mode determines file mode:
*       "r": read       "w": write      "a": append
*       "r+": read/write                "w+": open empty for read/write
*       "a+": read/append
*       Append "t" or "b" for text and binary mode. shflag determines the
*       sharing mode. Values are the same as for sopen().
*
*Entry:
*       char *file - file name to open
*       char *mode - mode of file access
*
*Exit:
*       returns pointer to stream
*       returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _tfsopen (
        const _TSCHAR *file,
        const _TSCHAR *mode
        ,int shflag
        )
{
        REG1 FILE *stream=NULL;
        REG2 FILE *retval=NULL;

        _VALIDATE_RETURN((file != NULL), EINVAL, NULL);
        _VALIDATE_RETURN((mode != NULL), EINVAL, NULL);
        _VALIDATE_RETURN((*mode != _T('\0')), EINVAL, NULL);

        /* Get a free stream */
        /* [NOTE: _getstream() returns a locked stream.] */

        if ((stream = _getstream()) == NULL)
        {
                errno = EMFILE;
                return(NULL);
        }

        __try {
        /* We deliberately don't hard-validate for emptry strings here. All other invalid
        path strings are treated as runtime errors by the inner code in _open and openfile.
        This is also the appropriate treatment here. Since fopen is the primary access point
        for file strings it might be subjected to direct user input and thus must be robust to
        that rather than aborting. The CRT and OS do not provide any other path validator (because
        WIN32 doesn't allow such things to exist in full generality).
        */
        if(*file==_T('\0'))
        {
            errno=EINVAL;
            return NULL;
        }

        /* open the stream */
#ifdef _UNICODE
        retval = _wopenfile(file,mode,shflag,stream);
#else  /* _UNICODE */
        retval = _openfile(file,mode,shflag,stream);
#endif  /* _UNICODE */

        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}


/***
*FILE *fopen(file, mode) - open a file
*
*Purpose:
*       Opens the file specified as a stream.  mode determines file mode:
*       "r": read       "w": write      "a": append
*       "r+": read/write                "w+": open empty for read/write
*       "a+": read/append
*       Append "t" or "b" for text and binary mode
*
*Entry:
*       char *file - file name to open
*       char *mode - mode of file access
*
*Exit:
*       returns pointer to stream
*       returns NULL if fails
*
*Exceptions:
*
*******************************************************************************/

FILE * __cdecl _tfopen (
        const _TSCHAR *file,
        const _TSCHAR *mode
        )
{
        return( _tfsopen(file, mode, _SH_DENYNO) );
}

/***
*errno_t _tfopen_s(pfile, file, mode) - open a file
*
*Purpose:
*       Opens the file specified as a stream.  mode determines file mode:
*       "r": read       "w": write      "a": append
*       "r+": read/write                "w+": open empty for read/write
*       "a+": read/append
*       Append "t" or "b" for text and binary mode
*       This is the secure version fopen - it opens the file in _SH_DENYRW
*       share mode.
*
*Entry:
*       FILE **pfile - Pointer to return the FILE handle into.
*       char *file - file name to open
*       char *mode - mode of file access
*
*Exit:
*       returns 0 on success & sets pfile
*       returns errno_t on failure.
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tfopen_s (
        FILE ** pfile,
        const _TSCHAR *file,
        const _TSCHAR *mode
        )
{
        _VALIDATE_RETURN_ERRCODE((pfile != NULL), EINVAL);
        *pfile = _tfsopen(file, mode, _SH_SECURE);

        if(*pfile != NULL)
            return 0;

        return errno;
}
