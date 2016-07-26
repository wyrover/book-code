/***
*wcsicmp.c - contains case-insensitive wide string comp routine _wcsicmp
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _wcsicmp()
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int _wcsicmp(dst, src) - compare wide-character strings, ignore case
*
*Purpose:
*       _wcsicmp perform a case-insensitive wchar_t string comparision.
*       _wcsicmp is independent of locale.
*
*Entry:
*       wchar_t *dst, *src - strings to compare
*
*Return:
*       Returns <0 if dst < src
*       Returns 0 if dst = src
*       Returns >0 if dst > src
*       Returns _NLSCMPERROR is something went wrong
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _wcsicmp_l (
        const wchar_t * dst,
        const wchar_t * src,
        _locale_t plocinfo
        )
{
    wchar_t f,l;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE)
    {
        do
        {
            f = __ascii_towlower(*dst);
            l = __ascii_towlower(*src);
            dst++;
            src++;
        }
        while ( (f) && (f == l) );
    }
    else
    {
        do
        {
            f = _towlower_l((unsigned short)*(dst++), _loc_update.GetLocaleT());
            l = _towlower_l((unsigned short)*(src++), _loc_update.GetLocaleT());
        }
        while ( (f) && (f == l) );
    }
    return (int)(f - l);
}

extern "C" int __cdecl _wcsicmp (
        const wchar_t * dst,
        const wchar_t * src
        )
{
    if (__locale_changed == 0)
    {
        wchar_t f,l;

        /* validation section */
        _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);

        do  {
            f = __ascii_towlower(*dst);
            l = __ascii_towlower(*src);
            dst++;
            src++;
        } while ( (f) && (f == l) );
        return (int)(f - l);
    }
    else
    {
        return _wcsicmp_l(dst, src, NULL);
    }
}

