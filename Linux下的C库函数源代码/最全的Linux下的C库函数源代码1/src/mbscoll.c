/***
*mbscoll.c - Collate MBCS strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Collate MBCS strings
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <internal.h>
#include <mbdata.h>
#include <mbctype.h>
#include <string.h>
#include <mbstring.h>
#include <locale.h>
#include <setlocal.h>


/***
* _mbscoll - Collate MBCS strings
*
*Purpose:
*       Collates two strings for lexical order.   Strings
*       are collated on a character basis, not a byte basis.
*
*Entry:
*       char *s1, *s2 = strings to collate
*
*Exit:
*       Returns <0 if s1 < s2
*       Returns  0 if s1 == s2
*       Returns >0 if s1 > s2
*       Returns _NLSCMPERROR is something went wrong
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _mbscoll_l(
        const unsigned char *s1,
        const unsigned char *s2,
        _locale_t plocinfo
        )
{
        int ret;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(s1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(s2 != NULL, EINVAL, _NLSCMPERROR);

        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return _strcoll_l((const char *)s1, (const char *)s2, plocinfo);

        if (0 == (ret = __crtCompareStringA(
                        _loc_update.GetLocaleT(),
                        _loc_update.GetLocaleT()->mbcinfo->mblcid,
                        SORT_STRINGSORT,
                        (LPCSTR)s1,
                        -1,
                        (LPSTR)s2,
                        -1,
                        _loc_update.GetLocaleT()->mbcinfo->mbcodepage )))
        {
            errno = EINVAL;

            return _NLSCMPERROR;
        }

        return ret - 2;

}

extern "C" int (__cdecl _mbscoll)(
        const unsigned char *s1,
        const unsigned char *s2
        )
{
    return _mbscoll_l(s1, s2, NULL);
}

#endif  /* _MBCS */
