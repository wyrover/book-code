/***
*_wctype.c - function versions of wctype macros
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file provides function versions of the wide character
*       classification and conversion macros in ctype.h.
*
*******************************************************************************/


/***
*wctype - Function versions of wctype macros
*
*Purpose:
*       Function versions of the wide char macros in ctype.h,
*       including isleadbyte and iswascii.  In order to define
*       these, we use a trick -- we undefine the macro so we can use the
*       name in the function declaration, then re-include the file so
*       we can use the macro in the definition part.
*
*       Functions defined:
*           iswalpha    iswupper     iswlower
*           iswdigit    iswxdigit    iswspace
*           iswpunct    iswalnum     iswprint
*           iswgraph    iswctrl      iswascii
*                                    isleadbyte
*
*Entry:
*       wchar_t c = character to be tested
*Exit:
*       returns non-zero = character is of the requested type
*                  0 = character is NOT of the requested type
*
*Exceptions:
*       None.
*
*******************************************************************************/

#include <ctype.h>
#include <cruntime.h>
#include <stdlib.h>
#include <locale.h>
#include <mbctype.h>
#include <mtdll.h>
#include <setlocal.h>

extern "C"
{
extern __inline int (__cdecl _isleadbyte_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);
    return (_loc_update.GetLocaleT()->locinfo->pctype[(unsigned char)(c)] & _LEADBYTE);
}

extern __inline int (__cdecl isleadbyte) (
        int c
        )
{
        return _isleadbyte_l(c, NULL);
}

extern __inline int (__cdecl _iswalpha_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswalpha_l(c, plocinfo);
}

extern __inline int (__cdecl iswalpha) (
        wint_t c
        )
{
        return iswalpha(c);
}

extern __inline int (__cdecl _iswupper_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswupper_l(c, plocinfo);
}

extern __inline int (__cdecl iswupper) (
        wint_t c
        )
{
        return iswupper(c);
}

extern __inline int (__cdecl _iswlower_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswlower_l(c, plocinfo);
}

extern __inline int (__cdecl iswlower) (
        wint_t c
        )
{
        return iswlower(c);
}

extern __inline int (__cdecl _iswdigit_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswdigit_l(c, plocinfo);
}

extern __inline int (__cdecl iswdigit) (
        wint_t c
        )
{
        return iswdigit(c);
}

extern __inline int (__cdecl _iswxdigit_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswxdigit_l(c, plocinfo);
}

extern __inline int (__cdecl iswxdigit) (
        wint_t c
        )
{
        return iswxdigit(c);
}

extern __inline int (__cdecl _iswspace_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswspace_l(c, plocinfo);
}

extern __inline int (__cdecl iswspace) (
        wint_t c
        )
{
        return iswspace(c);
}

extern __inline int (__cdecl _iswpunct_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswpunct_l(c, plocinfo);
}

extern __inline int (__cdecl iswpunct) (
        wint_t c
        )
{
        return iswpunct(c);
}

extern __inline int (__cdecl _iswalnum_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswalnum_l(c, plocinfo);
}

extern __inline int (__cdecl iswalnum) (
        wint_t c
        )
{
        return iswalnum(c);
}

extern __inline int (__cdecl _iswprint_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswprint_l(c, plocinfo);
}

extern __inline int (__cdecl iswprint) (
        wint_t c
        )
{
        return iswprint(c);
}

extern __inline int (__cdecl _iswgraph_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswgraph_l(c, plocinfo);
}

extern __inline int (__cdecl iswgraph) (
        wint_t c
        )
{
        return iswgraph(c);
}

extern __inline int (__cdecl _iswcntrl_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswcntrl_l(c, plocinfo);
}

extern __inline int (__cdecl iswcntrl) (
        wint_t c
        )
{
        return iswcntrl(c);
}

extern __inline int (__cdecl iswascii) (
        wint_t c
        )
{
        return iswascii(c);
}

extern __inline int (__cdecl _iswcsym_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswcsym_l(c, plocinfo);
}

extern __inline int (__cdecl __iswcsym) (
        wint_t c
        )
{
        return __iswcsym(c);
}

extern __inline int (__cdecl _iswcsymf_l) (
    wint_t c,
        _locale_t plocinfo
        )
{
        return _iswcsymf_l(c, plocinfo);
}

extern __inline int (__cdecl __iswcsymf) (
        wint_t c
        )
{
        return __iswcsymf(c);
}

}
