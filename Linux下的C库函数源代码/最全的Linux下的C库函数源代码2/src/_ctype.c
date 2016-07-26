/***
*_ctype.c - function versions of ctype macros
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This files provides function versions of the character
*       classification and conversion macros in ctype.h.
*
*******************************************************************************/

/***
*ctype - Function versions of ctype macros
*
*Purpose:
*       Function versions of the macros in ctype.h.  In order to define
*       these, we use a trick -- we use parentheses so we can use the
*       name in the function declaration without macro expansion, then
*       we use the macro in the definition part.
*
*       Functions defined:
*           isalpha     isupper     islower
*           isdigit     isxdigit    isspace
*           ispunct     isalnum     isprint
*           isgraph     isctrl      __isascii
*           __toascii   __iscsym    __iscsymf
*
*Entry:
*       int c = character to be tested
*Exit:
*       returns non-zero = character is of the requested type
*                  0 = character is NOT of the requested type
*
*Exceptions:
*       None.
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

#ifdef _DEBUG
#define __fast_ch_check(a,b)       _chvalidator(a,b)
#else  /* _DEBUG */
#define __fast_ch_check(a,b)       (__initiallocinfo.pctype[(a)] & (b))
#endif  /* _DEBUG */

extern "C"
{
extern __inline int (__cdecl _isalpha_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isalpha_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isalpha) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _ALPHA);
    }
    else
    {
        return (_isalpha_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isupper_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isupper_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isupper) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _UPPER);
    }
    else
    {
        return (_isupper_l)(c, NULL);
    }
}

extern __inline int (__cdecl _islower_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _islower_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl islower) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _LOWER);
    }
    else
    {
        return (_islower_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isdigit_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isdigit_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isdigit) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _DIGIT);
    }
    else
    {
        return (_isdigit_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isxdigit_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isxdigit_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isxdigit) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _HEX);
    }
    else
    {
        return (_isxdigit_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isspace_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isspace_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isspace) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _SPACE);
    }
    else
    {
        return (_isspace_l)(c, NULL);
    }
}

extern __inline int (__cdecl _ispunct_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _ispunct_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl ispunct) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _PUNCT);
    }
    else
    {
        return (_ispunct_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isalnum_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isalnum_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isalnum) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _ALPHA|_DIGIT);
    }
    else
    {
        return (_isalnum_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isprint_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isprint_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isprint) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _BLANK|_PUNCT|_ALPHA|_DIGIT);
    }
    else
    {
        return (_isprint_l)(c, NULL);
    }
}

extern __inline int (__cdecl _isgraph_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _isgraph_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl isgraph) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _PUNCT|_ALPHA|_DIGIT);
    }
    else
    {
        return (_isgraph_l)(c, NULL);
    }
}

extern __inline int (__cdecl _iscntrl_l) (
        int c,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _iscntrl_l(c, _loc_update.GetLocaleT());
}

extern __inline int (__cdecl iscntrl) (
        int c
        )
{
    if (__locale_changed == 0)
    {
        return __fast_ch_check(c, _CONTROL);
    }
    else
    {
        return (_iscntrl_l)(c, NULL);
    }
}

extern __inline int (__cdecl __isascii) (
        int c
        )
{
    return __isascii(c);
}

extern __inline int (__cdecl __toascii) (
        int c
        )
{
    return __toascii(c);
}

extern __inline int (__cdecl _iscsymf_l) (
        int c,
        _locale_t plocinfo
        )
{
        return (_isalpha_l)(c, plocinfo) || (c) == '_';
}
extern __inline int (__cdecl __iscsymf) (
        int c
        )
{
    return __iscsymf(c);
}

extern __inline int (__cdecl _iscsym_l) (
        int c,
        _locale_t plocinfo
        )
{
    return (_isalnum_l)(c, plocinfo) || (c) == '_';
}

extern __inline int (__cdecl __iscsym) (
        int c
        )
{
    return __iscsym((unsigned char)(c));
}
}
