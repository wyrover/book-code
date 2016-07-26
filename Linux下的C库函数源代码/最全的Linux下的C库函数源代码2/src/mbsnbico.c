/***
*mbsnbico.c - Collate n bytes of strings, ignoring case (MBCS)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Collate n bytes of strings, ignoring case (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <awint.h>
#include <mtdll.h>
#include <cruntime.h>
#include <internal.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <locale.h>
#include <setlocal.h>


/***
* _mbsnbicoll - Collate n bytes of strings, ignoring case (MBCS)
*
*Purpose:
*       Collates up to n bytes of two strings for lexical order.
*       Strings are collated on a character basis, not a byte basis.
*       Case of characters is not considered.
*
*Entry:
*       unsigned char *s1, *s2 = strings to collate
*       size_t n = maximum number of bytes to collate
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

extern "C" int __cdecl _mbsnbicoll_l(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n,
        _locale_t plocinfo
        )
{
        int ret;
        _LocaleUpdate _loc_update(plocinfo);

        if (n == 0)
            return 0;

        /* validation section */
        _VALIDATE_RETURN(s1 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(s2 != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(n <= INT_MAX, EINVAL, _NLSCMPERROR);

        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
            return _strnicoll_l((const char *)s1, (const char *)s2, n, plocinfo);

        if ( 0 == (ret = __crtCompareStringA(_loc_update.GetLocaleT(),  _loc_update.GetLocaleT()->mbcinfo->mblcid,
                                              SORT_STRINGSORT | NORM_IGNORECASE,
                                              (const char *)s1,
                                              (int)n,
                                              (char *)s2,
                                              (int)n,
                                              _loc_update.GetLocaleT()->mbcinfo->mbcodepage )) )
            return _NLSCMPERROR;

        return ret - 2;

}
extern "C" int __cdecl _mbsnbicoll(
        const unsigned char *s1,
        const unsigned char *s2,
        size_t n
        )
{
    return _mbsnbicoll_l(s1, s2, n, NULL);
}

#endif  /* _MBCS */
