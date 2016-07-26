/***
*tempnam.c - generate unique file name
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/


#include <cruntime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <internal.h>
#include <mtdll.h>
#include <tchar.h>
#include <dbgint.h>

#ifdef _MBCS
#include <mbstring.h>
#endif  /* _MBCS */

/* local tchar */
#ifdef _UNICODE
#define _tP_tmpdir _wP_tmpdir
#else  /* _UNICODE */
#define _tP_tmpdir _P_tmpdir
#endif  /* _UNICODE */

#ifdef _UNICODE
static wchar_t * _wstripquote (wchar_t *);
#else  /* _UNICODE */
static char * _stripquote (char *);
#endif  /* _UNICODE */

/***
*_TSCHAR *_tempnam(dir, prefix) - create unique file name
*
*Purpose:
*       Create a file name that is unique in the specified directory.
*       The semantics of directory specification is as follows:
*       Use the directory specified by the TMP environment variable
*       if that exists, else use the dir argument if non-NULL, else
*       use _P_tmpdir if that directory exists, else use the current
*       working directory), else return NULL.
*
*Entry:
*       _TSCHAR *dir - directory to be used for temp file if TMP env var
*                   not set
*       _TSCHAR *prefix - user provided prefix for temp file name
*
*Exit:
*       returns ptr to constructed file name if successful
*       returns NULL if unsuccessful
*
*Exceptions:
*
*******************************************************************************/
#ifdef _DEBUG

_TSCHAR * __cdecl _ttempnam (
        const _TSCHAR *dir,
        const _TSCHAR *pfx
        )
{
    return _ttempnam_dbg(dir, pfx, _NORMAL_BLOCK, NULL, 0);
}

_TSCHAR * __cdecl _ttempnam_dbg (
        const _TSCHAR *dir,
        const _TSCHAR *pfx,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )

#else  /* _DEBUG */

_TSCHAR * __cdecl _ttempnam (
        const _TSCHAR *dir,
        const _TSCHAR *pfx
        )

#endif  /* _DEBUG */

{
        REG1 _TSCHAR *ptr = NULL;
        REG2 unsigned int pfxlength = 0;
        _TSCHAR *s = NULL;
        _TSCHAR *pfin;
        unsigned int first;
        unsigned int bufsz;
        _TSCHAR * qptr = NULL;  /* ptr to TMP path with quotes stripped out */
        _TSCHAR * envbuf = NULL;  /* ptr to the env variable */
        errno_t save_errno;

        if ( !_mtinitlocknum( _TMPNAM_LOCK ))
                return NULL;

        /* try TMP path */
       if ( (_ERRCHECK_EINVAL(_tdupenv_s_crt(&envbuf, NULL, _T("TMP"))) == 0) &&
            (envbuf != NULL) &&
            (_taccess_s( envbuf, 0 ) == 0) )
       {
           ptr = envbuf;
           dir = ptr;
       }

        /* try stripping quotes out of TMP path */
#ifdef _UNICODE
        else if ( (ptr != NULL) && (qptr = _wstripquote(ptr)) &&
#else  /* _UNICODE */
        else if ( (ptr != NULL) && (qptr = _stripquote(ptr)) &&
#endif  /* _UNICODE */
                  (_taccess_s(qptr, 0) == 0 ) )
                dir = qptr;

        /* TMP path not available, use alternatives */
        else if (!( dir != NULL && ( _taccess_s( dir, 0 ) == 0 ) ) )
        /* do not "simplify" this depends on side effects!! */
        {
                if ( _taccess_s( _tP_tmpdir, 0 ) == 0 )
                    dir = _tP_tmpdir;
                else
                    dir = _T(".");
        }


        if (pfx)
                pfxlength = (unsigned)_tcslen(pfx);

        bufsz = (unsigned)_tcslen(dir) + pfxlength + 12;
        /* the 12 above allows for a backslash, 10 char temp string and
           a null terminator */

#ifdef _DEBUG
        if((s = _calloc_dbg(bufsz, sizeof(_TSCHAR), nBlockUse, szFileName, nLine)) == NULL)
#else  /* _DEBUG */
        if((s = calloc(bufsz, sizeof(_TSCHAR))) == NULL)
#endif  /* _DEBUG */

        {
                goto done2;
        }

        *s = _T('\0');
        _ERRCHECK(_tcscat_s( s, bufsz, dir ));
        pfin = (_TSCHAR *)&(dir[ _tcslen( dir ) - 1 ]);

#ifdef _MBCS
        if (*pfin == '\\') {
                if (pfin != _mbsrchr(dir,'\\'))
                        /* *pfin is second byte of a double-byte char */
                        _ERRCHECK(strcat_s( s, bufsz, "\\" ));
        }
        else if (*pfin != '/')
                _ERRCHECK(strcat_s( s, bufsz, "\\" ));
#else  /* _MBCS */
        if ( ( *pfin != _T('\\') ) && ( *pfin != _T('/') ) )
        {
                _ERRCHECK(_tcscat_s( s, bufsz, _T("\\") ));
        }
#endif  /* _MBCS */

        if ( pfx != NULL )
        {
                _ERRCHECK(_tcscat_s( s, bufsz, pfx ));
        }
        ptr = &s[_tcslen( s )];

        /*
        Re-initialize _tempoff if necessary.  If we don't re-init _tempoff, we
        can get into an infinate loop (e.g., (a) _tempoff is a big number on
        entry, (b) prefix is a long string (e.g., 8 chars) and all tempfiles
        with that prefix exist, (c) _tempoff will never equal first and we'll
        loop forever).

        [NOTE: To avoid a conflict that causes the same bug as that discussed
        above, _tempnam() uses _tempoff; tmpnam() uses _tmpoff]
        */

        _mlock(_TMPNAM_LOCK);   /* Lock access to _old_pfxlen and _tempoff */
        __try {

        if (_old_pfxlen < pfxlength)
                _tempoff = 1;
        _old_pfxlen = pfxlength;

        first = _tempoff;

        save_errno = errno;
        do {
                if ( (++_tempoff - first) > _TMP_MAX_S ) {
                        errno = save_errno;
                        free(s);
                        s = NULL;
                        goto done1;
                }
                /* the maximum length string returned by _ultot is 10 chars
                   (assuming 32-bit unsigned long) so there is enough room in
                   the tail of s (pointed to by ptr) for it */
                _ERRCHECK(_ultot_s( (unsigned long)_tempoff, ptr, bufsz - (ptr - s), 10 ));
                errno = 0;
        }
        while ( (_taccess_s( s, 0 ) == 0 ) || (errno == EACCES) );
        errno = save_errno;

        /* Common return */
done1:

        ; }
        __finally {
                _munlock(_TMPNAM_LOCK);     /* release tempnam lock */
        }

done2:
        _free_crt(envbuf);
        _free_crt(qptr);            /* free temp ptr, if non-NULL */
        return(s);
}



/***
*_stripquote() - Strip quotes out of a string
*
*Purpose:
*       This routine strips quotes out of a string.  This is necessary
*       in the case where a file/path name has embedded quotes (i.e.,
*       new file system.)
*
*       For example,
*                       c:\tmp\"a b c"\d --> c:\tmp\a b d\d
*
*       NOTE:  This routine makes a copy of the string since it may be
*       passed a pointer to an environment variable that shouldn't be
*       changed.  It is up to the caller to free up the memory (if the
*       return value is non-NULL).
*
*Entry:
*       _TSCHAR * ptr = pointer to string
*
*Exit:
*       _TSCHAR * ptr = pointer to copy of string with quotes gone.
*       NULL = no quotes in string.
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
static wchar_t * _wstripquote (
#else  /* _UNICODE */
static char * _stripquote (
#endif  /* _UNICODE */
        _TSCHAR * src
        )
{
        _TSCHAR * dst;
        _TSCHAR * ret;
        unsigned int q = 0;


        /* get a buffer for the new string */

        if ((dst = _calloc_crt((_tcslen(src)+1), sizeof(_TSCHAR))) == NULL)
                return(NULL);

        /* copy the string stripping out the quotes */

        ret = dst;          /* save base ptr */

        while (*src) {

                if (*src == _T('\"')) {
                        src++; q++;
                }
                else
                        *dst++ =  *src++;
        }

        if (q) {
                *dst = _T('\0');        /* final nul */
                return(ret);
        }
        else {
                _free_crt(ret);
                return(NULL);
        }

}

