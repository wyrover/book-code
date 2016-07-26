/***
*strlwr.c - routine to map upper-case characters in a string to lower-case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts all the upper case characters in a string to lower case,
*       in place.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <limits.h>     /* for INT_MAX */
#include <awint.h>
#include <dbgint.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*char *_strlwr(string) - map upper-case characters in a string to lower-case
*
*Purpose:
*       _strlwr() converts upper-case characters in a null-terminated string
*       to their lower-case equivalents.  Conversion is done in place and
*       characters other than upper-case letters are not modified.
*
*       In the C locale, this function modifies only 7-bit ASCII characters
*       in the range 0x41 through 0x5A ('A' through 'Z').
*
*       If the locale is not the 'C' locale, LCMapString() is used to do
*       the work.  Assumes enough space in the string to hold result.
*
*Entry:
*       char *string - string to change to lower case
*
*Exit:
*       input string address
*
*Exceptions:
*       The original string is returned unchanged on any error, and errno is set.
*
*******************************************************************************/

extern "C" char * __cdecl _strlwr_l (
        char * string,
        _locale_t plocinfo
        )
{
    _strlwr_s_l(string, (size_t)(-1), plocinfo);
    return string;
}

extern "C" char * __cdecl _strlwr (
        char * string
        )
{
    if (__locale_changed == 0)
    {
        char * cp;

        /* validation section */
        _VALIDATE_RETURN(string != NULL, EINVAL, NULL);

        for (cp=string; *cp; ++cp)
        {
            if ('A' <= *cp && *cp <= 'Z')
                *cp += 'a' - 'A';
        }

        return(string);
    }
    else
    {
        _strlwr_s_l(string, (size_t)(-1), NULL);
        return string;
    }
}

/***
*errno_t _strlwr_s(string, size_t) - map upper-case characters in a string to lower-case
*
*Purpose:
*       _strlwr_s() converts upper-case characters in a null-terminated string
*       to their lower-case equivalents.  Conversion is done in place and
*       characters other than upper-case letters are not modified.
*
*       In the C locale, this function modifies only 7-bit ASCII characters
*       in the range 0x41 through 0x5A ('A' through 'Z').
*
*       If the locale is not the 'C' locale, LCMapString() is used to do
*       the work.  Assumes enough space in the string to hold result.
*
*Entry:
*       char *string - string to change to lower case
*       size_t sizeInBytes - size of the destination buffer
*
*Exit:
*       the error code
*
*Exceptions:
*       The original string is returned unchanged on any error, and errno is set.
*
*******************************************************************************/

errno_t __cdecl _strlwr_s_l_stat (
        char * string,
        size_t sizeInBytes,
        _locale_t plocinfo
        )
{

    int dstsize;                /* size of dst string buffer (include null)  */
    unsigned char *dst;         /* destination string */
    errno_t e = 0;
    size_t stringlen;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(string != NULL, EINVAL);
    stringlen = strnlen(string, sizeInBytes);
    if (stringlen >= sizeInBytes)
    {
        _RESET_STRING(string, sizeInBytes);
        _RETURN_DEST_NOT_NULL_TERMINATED(string, sizeInBytes);
    }
    _FILL_STRING(string, sizeInBytes, stringlen + 1);

    if ( plocinfo->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
        char *cp;       /* traverses string for C locale conversion */

        for ( cp = string ; *cp ; ++cp )
            if ( ('A' <= *cp) && (*cp <= 'Z') )
                *cp -= 'A' - 'a';

        return 0;
    }   /* C locale */

    /* Inquire size of dst string */
    if ( 0 == (dstsize = __crtLCMapStringA(
                    plocinfo,
                    plocinfo->locinfo->lc_handle[LC_CTYPE],
                    LCMAP_LOWERCASE,
                    string,
                    -1,
                    NULL,
                    0,
                    plocinfo->locinfo->lc_codepage,
                    TRUE )) )
    {
        errno = EILSEQ;
        return errno;
    }

    if (sizeInBytes < (size_t)dstsize)
    {
        _RESET_STRING(string, sizeInBytes);
        _RETURN_BUFFER_TOO_SMALL(string, sizeInBytes);
    }

    /* Allocate space for dst */
    dst = (unsigned char *)_calloca(dstsize, sizeof(unsigned char));
    if (dst == NULL)
    {
        errno = ENOMEM;
        return errno;
    }

    /* Map src string to dst string in alternate case */
    if (__crtLCMapStringA(
                plocinfo,
                plocinfo->locinfo->lc_handle[LC_CTYPE],
                LCMAP_LOWERCASE,
                string,
                -1,
                (LPSTR)dst,
                dstsize,
                plocinfo->locinfo->lc_codepage,
                TRUE ) != 0)
    {
        /* copy dst string to return string */
        e = strcpy_s(string, sizeInBytes, (const char *)dst);
    }
    else
    {
        e = errno = EILSEQ;
    }

    _freea(dst);

    return e;
}

extern "C" errno_t __cdecl _strlwr_s_l (
        char * string,
        size_t sizeInBytes,
        _locale_t plocinfo
        )
{

    _LocaleUpdate _loc_update(plocinfo);

    return _strlwr_s_l_stat(string, sizeInBytes, _loc_update.GetLocaleT());
}

extern "C" errno_t __cdecl _strlwr_s(
        char * string,
        size_t sizeInBytes
        )
{
    return _strlwr_s_l(string, sizeInBytes, NULL);
}
