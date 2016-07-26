/***
*wcstombs.c - Convert wide char string to multibyte char string.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <limits.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <dbgint.h>
#include <errno.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*size_t __cdecl wcsncnt - count wide characters in a string, up to n.
*
*Purpose:
*       Internal local support function. Counts characters in string including NULL.
*       If NULL not found in n chars, then return n.
*
*Entry:
*       const wchar_t *string   - start of string
*       size_t n                - character count
*
*Exit:
*       returns number of wide characters from start of string to
*       NULL (inclusive), up to n.
*
*Exceptions:
*
*******************************************************************************/

static size_t __cdecl wcsncnt (
        const wchar_t *string,
        size_t cnt
        )
{
        size_t n = cnt+1;
        wchar_t *cp = (wchar_t *)string;

        while (--n && *cp)
            cp++;

        if (n && !*cp)
            return cp - string + 1;
        return cnt;
}

/***
*size_t wcstombs() - Convert wide char string to multibyte char string.
*
*Purpose:
*       Convert a wide char string into the equivalent multibyte char string,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       The destination string is null terminated only if the null terminator
*       is copied from the source string.
*
*Entry:
*       char *s            = pointer to destination multibyte char string
*       const wchar_t *pwc = pointer to source wide character string
*       size_t           n = maximum number of bytes to store in s
*
*Exit:
*       If s != NULL, returns    (size_t)-1 (if a wchar cannot be converted)
*       Otherwise:       Number of bytes modified (<=n), not including
*                    the terminating NUL, if any.
*
*Exceptions:
*       Returns (size_t)-1 if an error is encountered.
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" size_t __cdecl _wcstombs_l_helper (
        char * s,
        const wchar_t * pwcs,
        size_t n,
        _locale_t plocinfo
        )
{
    size_t count = 0;
    int i, retval;
    char buffer[MB_LEN_MAX];
    BOOL defused = 0;

    if (s && n == 0)
        /* dest string exists, but 0 bytes converted */
        return 0;

    /* validation section */
    _VALIDATE_RETURN(pwcs != NULL, EINVAL, (size_t)-1);
    _VALIDATE_RETURN(n <= INT_MAX, EINVAL, (size_t)-1);


    /* if destination string exists, fill it in */

    _LocaleUpdate _loc_update(plocinfo);

    if (s)
    {
        if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
        {
            /* C locale: easy and fast */
            /* Actually, there are such wchar_t characters which are > 255,
             * but they can be transformed to a valid single byte char
             * (i.e. a char in the C locale case). Like, for example,
             * alternative digits in unicode like Arabic-Indic U+0660..U+0669.
             * The problem is that WideCharToMultiByte() does not translate those
             * wchar_t unless we pass the correct codepage (1256, Arabic).
             * See bug VSW:192653.
             */
            while(count < n)
            {
                if (*pwcs > 255)  /* validate high byte */
                {
                    errno = EILSEQ;
                    return (size_t)-1;  /* error */
                }
                s[count] = (char) *pwcs;
                if (*pwcs++ == L'\0')
                    return count;
                count++;
            }
            return count;
        } else {

            if (1 == _loc_update.GetLocaleT()->locinfo->mb_cur_max)
            {
                /* If SBCS, one wchar_t maps to one char */

                /* WideCharToMultiByte will compare past NULL - reset n */
                if (n > 0)
                    n = wcsncnt(pwcs, n);
                if ( ((count = WideCharToMultiByte( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                                    0,
                                                    pwcs,
                                                    (int)n,
                                                    s,
                                                    (int)n,
                                                    NULL,
                                                    &defused )) != 0) &&
                     (!defused) )
                {
                    if (*(s + count - 1) == '\0')
                        count--; /* don't count NUL */

                    return count;
                }

                errno = EILSEQ;
                return (size_t)-1;
            }
            else {

                /* If MBCS, wchar_t to char mapping unknown */

                /* Assume that usually the buffer is large enough */
                if ( ((count = WideCharToMultiByte( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                                    0,
                                                    pwcs,
                                                    -1,
                                                    s,
                                                    (int)n,
                                                    NULL,
                                                    &defused )) != 0) &&
                     (!defused) )
                {
                    return count - 1; /* don't count NUL */
                }

                if (defused || GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                {
                    errno = EILSEQ;
                    return (size_t)-1;
                }

                /* buffer not large enough, must do char by char */
                while (count < n)
                {
                    if ( ((retval = WideCharToMultiByte( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                                         0,
                                                         pwcs,
                                                         1,
                                                         buffer,
                                                         _loc_update.GetLocaleT()->locinfo->mb_cur_max,
                                                         NULL,
                                                         &defused )) == 0)
                         || defused )
                    {
                        errno = EILSEQ;
                        return (size_t)-1;
                    }

                    /* enforce this for prefast */
                    if (retval < 0 ||
                        retval > _countof(buffer))
                    {
                        errno = EILSEQ;
                        return (size_t)-1;
                    }

                    if (count + retval > n)
                        return count;

                    for (i = 0; i < retval; i++, count++) /* store character */
                        if((s[count] = buffer[i])=='\0')
                            return count;

                    pwcs++;
                }

                return count;
            }
        }
    }
    else { /* s == NULL, get size only, pwcs must be NUL-terminated */
        if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
            return wcslen(pwcs);
        else {
            if ( ((count = WideCharToMultiByte( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                                0,
                                                pwcs,
                                                -1,
                                                NULL,
                                                0,
                                                NULL,
                                                &defused )) == 0) ||
                 (defused) )
            {
                errno = EILSEQ;
                return (size_t)-1;
            }

            return count - 1;
        }
    }

}

extern "C" size_t __cdecl _wcstombs_l (
        char * s,
        const wchar_t * pwcs,
        size_t n,
        _locale_t plocinfo
        )
{
    return _wcstombs_l_helper(s, pwcs, n, plocinfo);
}

extern "C" size_t __cdecl wcstombs (
        char * s,
        const wchar_t * pwcs,
        size_t n
        )
{
    return _wcstombs_l_helper(s, pwcs, n, NULL);
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
*       size_t *pConvertedChars = Number of bytes modified including the terminating NULL
*                                 This pointer can be NULL.
*       char *dst = pointer to destination multibyte char string
*       size_t sizeInBytes = size of the destination buffer
*       const wchar_t *src = pointer to source wide character string
*       size_t n = maximum number of bytes to store in s (not including the terminating NULL)
*
*Exit:
*       The error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" errno_t __cdecl _wcstombs_s_l (
        size_t *pConvertedChars,
        char * dst,
        size_t sizeInBytes,
        const wchar_t * src,
        size_t n,
        _locale_t plocinfo
        )
{
    size_t retsize;
    errno_t retvalue = 0;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE((dst != NULL && sizeInBytes > 0) || (dst == NULL && sizeInBytes == 0), EINVAL);
    if (dst != NULL)
    {
        _RESET_STRING(dst, sizeInBytes);
    }

    if (pConvertedChars != NULL)
    {
        *pConvertedChars = 0;
    }

    retsize = _wcstombs_l_helper(dst, src, (n > sizeInBytes ? sizeInBytes : n), plocinfo);

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
        /* return error if the string does not fit, unless n == _TRUNCATE */
        if (retsize > sizeInBytes)
        {
            if (n != _TRUNCATE)
            {
                _RESET_STRING(dst, sizeInBytes);
                _VALIDATE_RETURN_ERRCODE(sizeInBytes > retsize, ERANGE);
            }
            retsize = sizeInBytes;
            retvalue = STRUNCATE;
        }

        /* ensure the string is null terminated */
        dst[retsize - 1] = '\0';
    }

    if (pConvertedChars != NULL)
    {
        *pConvertedChars = retsize;
    }

    return retvalue;
}

extern "C" errno_t __cdecl wcstombs_s (
        size_t *pConvertedChars,
        char * dst,
        size_t sizeInBytes,
        const wchar_t * src,
        size_t n
        )
{
    return _wcstombs_s_l(pConvertedChars, dst, sizeInBytes, src, n, NULL);
}
