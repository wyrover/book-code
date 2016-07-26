/***
*stricoll.c - Collate locale strings without regard to case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <errno.h>
#include <awint.h>
#include <string.h>
#include <internal.h>
#include <windows.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int _stricoll() - Collate locale strings without regard to case
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information
*       without regard to case.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
*
*Exit:
*       Less than 0    = first string less than second string
*       0              = strings are equal
*       Greater than 0 = first string greater than second string
*       Returns _NLSCMPERROR is something went wrong
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _stricoll_l (
        const char *_string1,
        const char *_string2,
        _locale_t plocinfo
        )
{
    int ret;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
    {
        return _stricmp(_string1, _string2);
    }

    if ( 0 == (ret = __crtCompareStringA(_loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                    SORT_STRINGSORT | NORM_IGNORECASE,
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

extern "C" int __cdecl _stricoll (
        const char *_string1,
        const char *_string2
        )
{
    if (__locale_changed == 0)
    {
        return _stricmp(_string1, _string2);
    }
    else
    {
        return _stricoll_l(_string1, _string2, NULL);
    }

}
