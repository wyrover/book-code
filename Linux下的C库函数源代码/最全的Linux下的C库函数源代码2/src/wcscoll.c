/***
*wcscoll.c - Collate wide-character locale strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information.
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
*int wcscoll() - Collate wide-character locale strings
*
*Purpose:
*       Compare two wchar_t strings using the locale LC_COLLATE information.
*       In the C locale, wcscmp() is used to make the comparison.
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

extern "C" int __cdecl _wcscoll_l (
        const wchar_t *_string1,
        const wchar_t *_string2,
        _locale_t plocinfo
        )
{
    int ret;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
        return (wcscmp(_string1, _string2));

    if ( 0 == (ret = __crtCompareStringW(
                    _loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                    SORT_STRINGSORT,
                    _string1,
                    -1,
                    _string2,
                    -1,
                    _loc_update.GetLocaleT()->locinfo->lc_collate_cp)) )
    {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    return (ret - 2);

}

extern "C" int __cdecl wcscoll (
        const wchar_t *_string1,
        const wchar_t *_string2
        )
{
    if (__locale_changed == 0)
    {
        /* validation section */
        _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR);

        return (wcscmp(_string1, _string2));
    }
    else
    {
        return _wcscoll_l(_string1, _string2, NULL);
    }
}
