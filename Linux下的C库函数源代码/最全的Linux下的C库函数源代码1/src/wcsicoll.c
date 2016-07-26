/***
*wcsicoll.c - Collate wide-character locale strings without regard to case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*
*******************************************************************************/


#include <cruntime.h>
#include <internal.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <awint.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int _wcsicoll() - Collate wide-character locale strings without regard to case
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information
*       without regard to case.
*       In the C locale, _wcsicmp() is used to make the comparison.
*
*Entry:
*       const wchar_t *s1 = pointer to the first string
*       const wchar_t *s2 = pointer to the second string
*
*Exit:
*       -1 = first string less than second string
*        0 = strings are equal
*        1 = first string greater than second string
*       Returns _NLSCMPERROR is something went wrong
*       This range of return values may differ from other *cmp/*coll functions.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _wcsicoll_l (
        const wchar_t *_string1,
        const wchar_t *_string2,
        _locale_t plocinfo
        )
{
    int ret;
    wchar_t f, l;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR );
    _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR );

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
    {
        do
        {
            f = __ascii_towlower(*_string1);
            l = __ascii_towlower(*_string2);
            _string1++;
            _string2++;
        }
        while ( (f) && (f == l) );

        return (int)(f - l);
    }

    if ( 0 == (ret = __crtCompareStringW(
                    _loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                    SORT_STRINGSORT | NORM_IGNORECASE,
                    _string1,
                    -1,
                    _string2,
                    -1,
                    _loc_update.GetLocaleT()->locinfo->lc_codepage)) )
    {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    return (ret - 2);
}

extern "C" int __cdecl _wcsicoll (
        const wchar_t *_string1,
        const wchar_t *_string2
        )
{
    if (__locale_changed == 0)
    {
        wchar_t f,l;

        /* validation section */
        _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR );
        _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR );

        do
        {
            f = __ascii_towlower(*_string1);
            l = __ascii_towlower(*_string2);
            _string1++;
            _string2++;
        }
        while ( (f) && (f == l) );

        return (int)(f - l);
    }
    else
    {
        return _wcsicoll_l(_string1, _string2, NULL);
    }

}
