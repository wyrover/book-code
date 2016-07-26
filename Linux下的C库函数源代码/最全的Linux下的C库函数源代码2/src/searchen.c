/***
*searchenv.c - find a file using paths from an environment variable
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       to search a set a directories specified by an environment variable
*       for a specified file name.  If found the full path name is returned.
*
*******************************************************************************/

#include <cruntime.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <internal.h>
#include <tchar.h>
#include <dbgint.h>

/***
*_searchenv_s - search for file along paths from environment variable
*
*Purpose:
*       to search for a specified file in the directory(ies) specified by
*       a given environment variable, and, if found, to return the full
*       path name of the file.  The file is first looked for in the current
*       working directory, prior to looking in the paths specified by env_var.
*
*Entry:
*       const _TSCHAR * fname - name of file to search for
*       const _TSCHAR * env_var - name of environment variable to use for paths
*       _TSCHAR * path - pointer to storage for the constructed path name
*       size_t sz - the size of the path buffer
*
*Exit:
*       returns 0 on success & sets pfile
*       returns errno_t on failure.
*       The path parameter is filled with the fullpath of found file on success
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tsearchenv_s (
        const _TSCHAR *fname,
        const _TSCHAR *env_var,
        _TSCHAR *path,
        size_t sz
        )
{
        register _TSCHAR *p;
        register int c;
        _TSCHAR *envbuf = NULL;
        _TSCHAR *env_p, *save_env_p;
        size_t len;
        _TSCHAR pathbuf[_MAX_PATH + 4];
        _TSCHAR * pbuf = NULL;
        size_t fnamelen, buflen;
        errno_t save_errno;
        int ret;
        errno_t retvalue = 0;

        _VALIDATE_RETURN_ERRCODE( (path != NULL), EINVAL);
        _VALIDATE_RETURN_ERRCODE( (sz > 0), EINVAL);
        if (fname == NULL)
        {
            *path = _T('\0');
            _VALIDATE_RETURN_ERRCODE( (fname != NULL), EINVAL);
        }

        /* special case: fname is an empty string: just return an empty path, errno is set to ENOENT */
        if (fname[0] == 0)
        {
            *path = _T('\0');
            errno = ENOENT;
            retvalue = errno;
            goto cleanup;
        }

        save_errno = errno;
        ret = _taccess_s(fname, 0);
        errno = save_errno;

        if (ret == 0 ) {

            /* exists, convert it to a fully qualified pathname and
               return */
            if ( _tfullpath(path, fname, sz) == NULL )
            {
                *path = _T('\0'); /* fullpath will set errno in this case */
                retvalue = errno;
                goto cleanup;
            }

            retvalue = 0;
            goto cleanup;
        }

        if (_ERRCHECK_EINVAL(_tdupenv_s_crt(&envbuf, NULL, env_var)) != 0 || envbuf == NULL) {
            /* no such environment var. and not in cwd, so return empty
               string, set errno to ENOENT */
            *path = _T('\0');
            errno = ENOENT;
            retvalue = errno;
            goto cleanup;
        }

        env_p = envbuf;
        fnamelen = _tcslen(fname);
        pbuf = pathbuf;
        buflen = _countof(pathbuf);

        if(fnamelen >= buflen)
        {
            /* +2 for the trailing '\' we may need to add & the '\0' */
            buflen = _tcslen(env_p) + fnamelen + 2;
            pbuf = (_TSCHAR *)_calloc_crt( buflen, sizeof(_TSCHAR));
            if(!pbuf)
            {
                 *path = _T('\0');
                 errno = ENOMEM;
                 retvalue = errno;
                 goto cleanup;
            }
        }

        save_errno = errno;

        while(env_p)
        {
            save_env_p = env_p;
            env_p = _tgetpath(env_p, pbuf, buflen - fnamelen - 1);

            if( env_p == NULL && pbuf == pathbuf && errno == ERANGE)
            {
                buflen = _tcslen(save_env_p) + fnamelen + 2;
                pbuf = (_TSCHAR *)_calloc_crt( buflen, sizeof(_TSCHAR));
                if(!pbuf)
                {
                     *path = _T('\0');
                     errno = ENOMEM;
                     retvalue = errno;
                     goto cleanup;
                }
                env_p = _tgetpath(save_env_p, pbuf, buflen - fnamelen);
            }

            if(env_p == NULL || *pbuf == _T('\0'))
                break;

            /* path now holds nonempty pathname from env_p, concatenate
               the file name and go */
            /* If we reached here, we know that buflen is enough to hold
            the concatenation. If not, the getpath would have failed */

            len = _tcslen(pbuf);
            p = pbuf + len;
            if ( ((c = *(p - 1)) != _T('/')) && (c != _T('\\')) &&
                 (c != _T(':')) )
            {
                /* add a trailing '\' */
                *p++ = _T('\\');
                len++;
            }
            /* p now points to character following trailing '/', '\'
               or ':' */

            _ERRCHECK(_tcscpy_s(p, buflen - (p - pbuf), fname));

            if ( _taccess_s(pbuf, 0) == 0 ) {
                /* found a match, copy the full pathname into the caller's
                   buffer */
                if(len + fnamelen >= sz) {
                    *path = _T('\0');

                    if(pbuf != pathbuf)
                        _free_crt(pbuf);

                    errno = ERANGE;
                    retvalue = errno;
                    goto cleanup;
                }

                errno = save_errno;

                _ERRCHECK(_tcscpy_s(path, sz, pbuf));

                if(pbuf != pathbuf)
                    _free_crt(pbuf);

                retvalue = 0;
                goto cleanup;
            }

        }
        /* if we get here, we never found it, return empty string */
        *path = _T('\0');
        errno = ENOENT;
        retvalue = errno;

cleanup:
        if(pbuf != pathbuf)
            _free_crt(pbuf);

        _free_crt(envbuf);

        return retvalue;
}

/***
*_searchenv() - search for file along paths from environment variable
*
*Purpose:
*       to search for a specified file in the directory(ies) specified by
*       a given environment variable, and, if found, to return the full
*       path name of the file.  The file is first looked for in the current
*       working directory, prior to looking in the paths specified by env_var.
*
*Entry:
*       fname - name of file to search for
*       env_var - name of environment variable to use for paths
*       path - pointer to storage for the constructed path name
*
*Exit:
*       path - pointer to constructed path name, if the file is found, otherwise
*              it points to the empty string.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _tsearchenv (
        const _TSCHAR *fname,
        const _TSCHAR *env_var,
        _TSCHAR *path
        )
{
    _tsearchenv_s(fname, env_var, path, _MAX_PATH);
}
