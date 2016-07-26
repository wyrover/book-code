/***
*strncoll.c - Collate locale strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <locale.h>
#include <errno.h>
#include <awint.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int _strncoll() - Collate locale strings
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
*       size_t count - maximum number of characters to compare
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
extern "C" int __cdecl _strncoll_l (
        const char *_string1,
        const char *_string2,
        size_t count,
        _locale_t plocinfo
        )
{
    int ret;

    if ( !count )
        return 0;

    /* validation section */
    _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(count <= INT_MAX, EINVAL, _NLSCMPERROR);

    _LocaleUpdate _loc_update(plocinfo);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
    {
        return strncmp(_string1, _string2, count);
    }

    if ( 0 == (ret = __crtCompareStringA(
                    _loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                    SORT_STRINGSORT,
                    _string1,
                    (int)count,
                    _string2,
                    (int)count,
                    _loc_update.GetLocaleT()->locinfo->lc_collate_cp)) )
    {
        errno = EINVAL;
        return _NLSCMPERROR;
    }

    return (ret - 2);
}

extern "C" int __cdecl _strncoll (
        const char *_string1,
        const char *_string2,
        size_t count
        )
{
    if (__locale_changed == 0)
    {
        /* validation section */
        _VALIDATE_RETURN(_string1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(_string2 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(count <= INT_MAX, EINVAL, _NLSCMPERROR);

        return strncmp(_string1, _string2, count);
    }
    else
    {
        return _strncoll_l(_string1, _string2, count, NULL);
    }
}
