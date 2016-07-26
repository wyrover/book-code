/***
*wcsupr.c - routine to map lower-case characters in a wchar_t string
*       to upper-case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts all the lower case characters in a wchar_t string
*       to upper case, in place.
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <ctype.h>
#include <awint.h>
#include <dbgint.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*wchar_t *_wcsupr(string) - map lower-case characters in a string to upper-case
*
*Purpose:
*       wcsupr converts lower-case characters in a null-terminated wchar_t
*       string to their upper-case equivalents.  The result may be longer or
*       shorter than the original string.  Assumes enough space in string
*       to hold the result.
*
*Entry:
*       wchar_t *wsrc - wchar_t string to change to upper case
*
*Exit:
*       input string address
*
*Exceptions:
*       on an error, the original string is unaltered, and errno is set
*
*******************************************************************************/

extern "C" wchar_t * __cdecl _wcsupr_l (
        wchar_t * wsrc,
        _locale_t plocinfo
        )
{
    _wcsupr_s_l(wsrc, (size_t)(-1), plocinfo);
    return wsrc;
}

extern "C" wchar_t * __cdecl _wcsupr (
        wchar_t * wsrc
        )
{
    if (__locale_changed == 0)
    {
    wchar_t * p;

    /* validation section */
    _VALIDATE_RETURN(wsrc != NULL, EINVAL, NULL);

    for (p=wsrc; *p; ++p)
    {
            if (L'a' <= *p && *p <= L'z')
                    *p += (wchar_t)(L'A' - L'a');
    }

        return wsrc;
    }
    else
    {
        _wcsupr_s_l(wsrc, (size_t)-1, NULL);
        return wsrc;
    }
}

/***
*errno_t _wcsupr_s(string, size_t) - map lower-case characters in a string to upper-case
*
*Purpose:
*       wcsupr converts lower-case characters in a null-terminated wchar_t
*       string to their upper-case equivalents.  The result may be longer or
*       shorter than the original string.
*
*Entry:
*       wchar_t *wsrc - wchar_t string to change to upper case
*       size_t sizeInWords - size of the destination buffer
*
*Exit:
*       the error code
*
*Exceptions:
*       on an error, the original string is unaltered, and errno is set
*
*******************************************************************************/

static errno_t __cdecl _wcsupr_s_l_stat (
        wchar_t * wsrc,
        size_t sizeInWords,
        _locale_t plocinfo
        )
{

    wchar_t *p;             /* traverses string for C locale conversion */
    wchar_t *wdst;          /* wide version of string in alternate case */
    int dstsize;            /* size in wide chars of wdst string buffer (include null) */
    errno_t e = 0;
    size_t stringlen;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(wsrc != NULL, EINVAL);
    stringlen = wcsnlen(wsrc, sizeInWords);
    if (stringlen >= sizeInWords)
    {
        _RESET_STRING(wsrc, sizeInWords);
        _RETURN_DEST_NOT_NULL_TERMINATED(wsrc, sizeInWords);
    }
    _FILL_STRING(wsrc, sizeInWords, stringlen + 1);

    if ( plocinfo->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
    {
        for ( p = wsrc ; *p ; p++ )
        {
            if ( (*p >= (wchar_t)L'a') && (*p <= (wchar_t)L'z') )
                *p = *p - (L'a' - L'A');
        }
        return 0;
    }   /* C locale */


    /* Inquire size of wdst string */
    if ( (dstsize = __crtLCMapStringW(plocinfo,
                    plocinfo->locinfo->lc_handle[LC_CTYPE],
                    LCMAP_UPPERCASE,
                    wsrc,
                    -1,
                    NULL,
                    0,
                    plocinfo->locinfo->lc_codepage )) == 0 )
    {
        errno = EILSEQ;
        return errno;
    }

    if (sizeInWords < (size_t)dstsize)
    {
        _RESET_STRING(wsrc, sizeInWords);
        _RETURN_BUFFER_TOO_SMALL(wsrc, sizeInWords);
    }

    /* Allocate space for wdst */
    wdst = (wchar_t *)_calloca(dstsize, sizeof(wchar_t));
    if (wdst == NULL)
    {
        errno = ENOMEM;
        return errno;
    }

    /* Map wrc string to wide-character wdst string in alternate case */
    if (__crtLCMapStringW(plocinfo,
                plocinfo->locinfo->lc_handle[LC_CTYPE],
                LCMAP_UPPERCASE,
                wsrc,
                -1,
                wdst,
                dstsize,
                plocinfo->locinfo->lc_codepage ) != 0)
    {
        /* Copy wdst string to user string */
        e = wcscpy_s(wsrc, sizeInWords, wdst);
    }
    else
    {
        e = errno = EILSEQ;
    }

    _freea(wdst);

    return e;
}

extern "C" errno_t __cdecl _wcsupr_s_l (
        wchar_t * wsrc,
        size_t sizeInWords,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _wcsupr_s_l_stat(wsrc, sizeInWords, _loc_update.GetLocaleT());
}


extern "C" errno_t __cdecl _wcsupr_s (
        wchar_t * wsrc,
        size_t sizeInWords
        )
{
    return _wcsupr_s_l(wsrc, sizeInWords, NULL);
}
