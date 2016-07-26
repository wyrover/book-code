/***
*wctomb.c - Convert wide character to multibyte character.
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
#include <locale.h>
#include <internal.h>
#include <limits.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*errno_t wctomb_s() - Convert wide character to multibyte character.
*
*Purpose:
*       Convert a wide character into the equivalent multi-byte character,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       int *retvalue = pointer to a useful return value:
*           if s == NULL && sizeInBytes == 0: number of bytes needed for the conversion
*           if s == NULL && sizeInBytes > 0: the state information
*           if s != NULL : number of bytes used for the conversion
*           The pointer can be null.
*       char *s = pointer to multibyte character
*       size_t sizeInBytes = size of the destination buffer
*       wchar_t wchar = source wide character
*
*Exit:
*       The error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _wctomb_s_l (
        int *pRetValue,
        char *dst,
        size_t sizeInBytes,
        wchar_t wchar,
        _locale_t plocinfo
        )
{
    if (dst == NULL && sizeInBytes > 0)
    {
        /* indicate do not have state-dependent encodings */
        if (pRetValue != NULL)
        {
            *pRetValue = 0;
        }
        return 0;
    }

    if (pRetValue != NULL)
    {
        *pRetValue = -1;
    }

    /* validation section */
    /* we need to cast sizeInBytes to int, so we make sure we are not going to truncate sizeInBytes */
    _VALIDATE_RETURN_ERRCODE(sizeInBytes <= INT_MAX, EINVAL);


    _LocaleUpdate _loc_update(plocinfo);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
    {
        if ( wchar > 255 )  /* validate high byte */
        {
            if (dst != NULL && sizeInBytes > 0)
            {
                memset(dst, 0, sizeInBytes);
            }
            errno = EILSEQ;
            return errno;
        }

        if (dst != NULL)
        {
            _VALIDATE_RETURN_ERRCODE(sizeInBytes > 0, ERANGE);
            *dst = (char) wchar;
        }
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

        if ( ((size = WideCharToMultiByte( _loc_update.GetLocaleT()->locinfo->lc_codepage,
                                           0,
                                           &wchar,
                                           1,
                                           dst,
                                           (int)sizeInBytes,
                                           NULL,
                                           &defused) ) == 0) ||
             (defused) )
        {
            if (size == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (dst != NULL && sizeInBytes > 0)
                {
                    memset(dst, 0, sizeInBytes);
                }
                _VALIDATE_RETURN_ERRCODE(("Buffer too small", 0), ERANGE);
            }
            errno = EILSEQ;
            return errno;
        }

        if (pRetValue != NULL)
        {
            *pRetValue = size;
        }
        return 0;
    }

}

extern "C" errno_t __cdecl wctomb_s (
        int *pRetValue,
        char *dst,
        size_t sizeInBytes,
        wchar_t wchar
        )
{
        return _wctomb_s_l(pRetValue, dst, sizeInBytes, wchar, NULL);
}

/***
*int wctomb() - Convert wide character to multibyte character.
*
*Purpose:
*       Convert a wide character into the equivalent multi-byte character,
*       according to the LC_CTYPE category of the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       char *s          = pointer to multibyte character
*       wchar_t wchar        = source wide character
*
*Exit:
*       If s = NULL, returns 0, indicating we only use state-independent
*       character encodings.
*       If s != NULL, returns:
*                   -1 (if error) or number of bytes comprising
*                   converted mbc
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _wctomb_l (
        char *s,
        wchar_t wchar,
        _locale_t plocinfo
        )
{
    int retval = -1;
    errno_t e;
    _LocaleUpdate _loc_update(plocinfo);

    e = _wctomb_s_l(&retval, s, _loc_update.GetLocaleT()->locinfo->mb_cur_max, wchar, _loc_update.GetLocaleT());
    return (e == 0 ? retval : -1);
}

extern "C" int __cdecl wctomb (
        char *s,
        wchar_t wchar
        )
{
    int retval = -1;
    errno_t e;

    e = _wctomb_s_l(&retval, s, MB_CUR_MAX, wchar, NULL);
    return (e == 0 ? retval : -1);
}
