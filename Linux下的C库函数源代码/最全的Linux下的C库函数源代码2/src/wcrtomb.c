/***
*wcrtomb.c - Convert wide character to multibyte character, with locale.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a wide character into the equivalent multibyte character.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <errno.h>
#include <dbgint.h>
#include <ctype.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>
#include <wchar.h>
#include <string.h>              /* for memcpy_s */
#include <limits.h>              /* for MB_LEN_MAX */
#include <stdio.h>               /* for EOF */

/***
*errno_t _wcrtomb_s_l() - Helper function to convert wide character to multibyte character.
*
*Purpose:
*       Convert a wide character into the equivalent multi-byte character,
*       according to the specified LC_CTYPE category, or the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       int *pRetValue      = the number of chars written (-1 in error case)
*       char *dst           = pointer to multibyte character
*       size_t sizeInBytes  = size of the destinarion buffer
*       wchar_t wchar       = source wide character
*       mbstate_t *pst      = pointer to state (not used)
*       _locale_t plocinfo  = locale info
*
*Exit:
*       Returns:
*       Value of errno if errors, 0 otherwise. *pRetValue is set to -1 in error case.
*
*Exceptions:
*
*******************************************************************************/

static errno_t __cdecl _wcrtomb_s_l (
    int *pRetValue,
    char *dst,
    size_t sizeInBytes,
    wchar_t wchar,
    mbstate_t *pst,
    _locale_t plocinfo
)
{
    _ASSERTE (dst != NULL && sizeInBytes > 0);

    _LocaleUpdate _loc_update(plocinfo);
    _ASSERTE (_loc_update.GetLocaleT()->locinfo->mb_cur_max == 1 || _loc_update.GetLocaleT()->locinfo->mb_cur_max == 2);
    if(pst!=NULL)
    {
        *pst=0;
    }

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
    {
        if ( wchar > 255 )  /* validate high byte */
        {
            errno = EILSEQ;
            if (pRetValue != NULL)
            {
                *pRetValue = -1;
            }
            return errno;
        }

        *dst = (char) wchar;
        if (pRetValue != NULL)
        {
            *pRetValue = 1;
        }
        return 0;
    }
    else
    {
        int size;
        BOOL defused = 0;

        if ( ((size = WideCharToMultiByte(_loc_update.GetLocaleT()->locinfo->lc_codepage,
                                          0,
                                          &wchar,
                                          1,
                                          dst,
                                          (int)sizeInBytes,
                                          NULL,
                                          &defused)) == 0) ||
                (defused) )
        {
            errno = EILSEQ;
            if (pRetValue != NULL)
            {
                *pRetValue = -1;
            }
            return errno;
        }

        if (pRetValue != NULL)
        {
            *pRetValue = size;
        }
        return 0;
    }
}

/***
*errno_t wcrtomb_s(retValue, dst, sizeInBytes, wchar, pmbst) - translate wchar_t to multibyte, restartably
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

extern "C" errno_t __cdecl wcrtomb_s(
        size_t *pRetValue,
        char *dst,
        size_t sizeInBytes,
        wchar_t wchar,
        mbstate_t *pst
        )
{
    int retValue = -1;
    errno_t e;

    /* validation section */
    /* note that we do not force sizeInBytes > 0 in the dst != NULL case, because we do not need to add
     * null terminator, due that dst will receive a character and not a string
     */
    _VALIDATE_RETURN_ERRCODE((dst == NULL && sizeInBytes == 0) || (dst != NULL), EINVAL);

    if (dst == NULL)
    {
        char buf[MB_LEN_MAX];
        e = _wcrtomb_s_l(&retValue, buf, MB_LEN_MAX, wchar, pst, NULL);
    }
    else
    {
        e = _wcrtomb_s_l(&retValue, dst, sizeInBytes, wchar, pst, NULL);
    }

    if (pRetValue != NULL)
    {
        (*pRetValue) = (size_t)retValue;
    }
    return e;
}

extern "C" size_t __cdecl wcrtomb(
        char *dst,
        wchar_t wchar,
        mbstate_t *mbst
        )
{
    size_t retValue = -1;

    wcrtomb_s(&retValue, dst, (dst == NULL ? 0 : MB_LEN_MAX), wchar, mbst);
    return retValue;
}

/***
*errno_t wcsrtombs_s(retValue, dst, sizeInBytes, pwcs, n, pst) - translate wide char string to multibyte
*       string
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

/* Helper shared by secure and non-secure functions. */

extern "C" size_t __cdecl _wcsrtombs_helper(
        char *s,
        const wchar_t **pwcs,
        size_t n,
        mbstate_t *pst
        )
{
    /* validation section */
    _VALIDATE_RETURN(pwcs != NULL, EINVAL, (size_t)-1);

    char buf[MB_LEN_MAX];
    int i = 0;
    size_t nc = 0;
    const wchar_t *wcs = *pwcs;
    _LocaleUpdate _loc_update(NULL);

    if (s == NULL)
    {
        for (; ; nc += i, ++wcs)
        {
            /* translate but don't store */
            _wcrtomb_s_l(&i, buf, MB_LEN_MAX, *wcs, pst, _loc_update.GetLocaleT());
            if (i <= 0)
            {
                return ((size_t)-1);
            }
            else if (buf[i - 1] == '\0')
            {
                return (nc + i - 1);
            }
        }
    }

    for (; 0 < n; nc += i, ++wcs, s += i, n -= i)
    {
        /* translate and store */
        char *t = NULL;

        if (n < (size_t)_loc_update.GetLocaleT()->locinfo->mb_cur_max)
        {
            t = buf;
        }
        else
        {
            t = s;
        }

        _wcrtomb_s_l(&i, t, MB_LEN_MAX, *wcs, pst, _loc_update.GetLocaleT());
        if (i <= 0)
        {
            /* encountered invalid sequence */
            nc = (size_t)-1;
            break;
        }

        if (s == t)
        {
            /* do nothing */
        }
        else if (n < (size_t)i)
        {
            break;  /* won't all fit */
        }
        else
        {
            memcpy_s(s, n, buf, i);
        }

        if (s[i - 1] == '\0')
        {
            /* encountered terminating null */
            *pwcs = 0;
            return (nc + i - 1);
        }
    }

    *pwcs = wcs;
    return nc;
}

extern "C" size_t __cdecl wcsrtombs(
        char *s,
        const wchar_t **pwcs,
        size_t n,
        mbstate_t *pst
        )
{
    /* Call a non-deprecated helper to do the work. */

    return _wcsrtombs_helper(s, pwcs, n, pst);
}

/***
*errno_t wcstombs_s() - Convert wide char string to multibyte char string.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string,
*       according to the LC_CTYPE category of the current locale.
*
*       The destination string is always null terminated.
*
*Entry:
*       size_t *pRetValue = Number of bytes modified including the terminating NULL
*                           This pointer can be NULL.
*       char *dst = pointer to destination multibyte char string
*       size_t sizeInBytes = size of the destination buffer
*       const wchar_t *src = pointer to source wide character string
*       size_t n = maximum number of bytes to store in s (not including the terminating NULL)
*       mbstate_t *pmbst = pointer to state
*
*Exit:
*       The error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" errno_t __cdecl wcsrtombs_s(
        size_t *pRetValue,
        char *dst,
        size_t sizeInBytes,
        const wchar_t **src,
        size_t n,
        mbstate_t *pmbst
        )
{
    size_t retsize;

    if (pRetValue != NULL)
    {
        *pRetValue = -1;
    }

    /* validation section */
    _VALIDATE_RETURN_ERRCODE((dst == NULL && sizeInBytes == 0) || (dst != NULL && sizeInBytes > 0), EINVAL);
    if (dst != NULL)
    {
        _RESET_STRING(dst, sizeInBytes);
    }
    _VALIDATE_RETURN_ERRCODE(src != NULL, EINVAL);

    /* Call a non-deprecated helper to do the work. */
    retsize = _wcsrtombs_helper(dst, src, (n > sizeInBytes ? sizeInBytes : n), pmbst);

    if (retsize == (size_t)-1)
    {
        if (dst != NULL)
        {
            _RESET_STRING(dst, sizeInBytes);
        }
        return errno;
    }

    /* count the null terminator */
    retsize++;

    if (dst != NULL)
    {
        /* return error if the string does not fit */
        if (retsize > sizeInBytes)
        {
            _RESET_STRING(dst, sizeInBytes);
            _VALIDATE_RETURN_ERRCODE(sizeInBytes <= retsize, ERANGE);
        }

        /* ensure the string is null terminated */
        dst[retsize - 1] = '\0';
    }

    if (pRetValue != NULL)
    {
        *pRetValue = retsize;
    }

    return 0;
}

/***
*int wctob(wchar) - translate wint_t to one-byte multibyte
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl wctob(
    wint_t wchar
)
{
    if (wchar == WEOF)
    {
        return (EOF);
    }
    else
    {
        /* check for one-byte translation */
        int retValue = -1;
        char buf[MB_LEN_MAX];
        errno_t e;

        e = _wcrtomb_s_l(&retValue, buf, MB_LEN_MAX, wchar, NULL, NULL);
        if (e == 0 && retValue == 1)
        {
            return buf[0];
        }
        return EOF;
    }
}
