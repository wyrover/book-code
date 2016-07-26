/***
*mbsncol.c - Collate n characters of two MBCS strings
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Collate n characters of two MBCS strings
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
#include <limits.h>
#include <locale.h>
#include <setlocal.h>

/***
* _mbsncoll(s1, s2, n) - Collate n characters of two MBCS strings
*
*Purpose:
*       Collates up to n charcters of two strings for lexical order.
*       Strings are collated on a character basis, not a byte basis.
*
*Entry:
*       unsigned char *s1, *s2 = strings to collate
*       size_t n = maximum number of characters to collate
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

extern "C" int __cdecl _mbsncoll_l(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n,
        _locale_t plocinfo
        )
{
        int ret;
        size_t bcnt1, bcnt2;
        _LocaleUpdate _loc_update(plocinfo);

        if (n == 0)
            return 0;

        /* validation section */
        _VALIDATE_RETURN(s1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(s2 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(n <= INT_MAX, EINVAL, _NLSCMPERROR);

        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return _strncoll_l((const char *)s1, (const char *)s2, n, plocinfo);

        bcnt1 = _mbsnbcnt_l(s1, n, _loc_update.GetLocaleT());
        bcnt2 = _mbsnbcnt_l(s2, n, _loc_update.GetLocaleT());

        if ( 0 == (ret = __crtCompareStringA(
                        _loc_update.GetLocaleT(),
                        _loc_update.GetLocaleT()->mbcinfo->mblcid,
                        SORT_STRINGSORT,
                        (const char *)s1,
                        (int)bcnt1,
                        (char *)s2,
                        (int)bcnt2,
                        _loc_update.GetLocaleT()->mbcinfo->mbcodepage )) )
        {
            errno = EINVAL;
            return _NLSCMPERROR;
        }

        return ret - 2;

}

extern "C" int (__cdecl _mbsncoll)(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n
        )
{
    return _mbsncoll_l(s1, s2, n, NULL);
}
#endif  /* _MBCS */
