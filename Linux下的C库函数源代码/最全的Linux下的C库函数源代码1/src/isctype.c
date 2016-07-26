/***
*isctype.c - support is* ctype functions/macros for two-byte multibyte chars
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _isctype.c - support is* ctype functions/macros for
*       two-byte multibyte chars.
*
*******************************************************************************/

#include <stdio.h>
#include <cruntime.h>
#include <ctype.h>
#include <locale.h>
#include <awint.h>
#include <dbgint.h>
#include <mtdll.h>
#include <setlocal.h>


/*
 *  Use GetCharType() API so check that character type masks agree between
 *  ctype.h and winnls.h
 */
#if _UPPER   != C1_UPPER || \
        _LOWER   != C1_LOWER || \
        _DIGIT   != C1_DIGIT || \
        _SPACE   != C1_SPACE || \
        _PUNCT   != C1_PUNCT || \
        _CONTROL != C1_CNTRL
#error Character type masks do not agree in ctype and winnls
#endif  /* _UPPER   != C1_UPPER || \ */



/***
* __chvalidator
*
* Purpose:
*       This function is called by character testing functions in debug
*       versions. This function test for validation of c as character.
*       For improvement in performance, it is not used in non-debug
*       version.  It is available in the static single-thread non-debug
*       build, though, just in case C code that includes ctype.h is compiled
*       /D_DEBUG /ML.
*
*******************************************************************************/

#if defined (_DEBUG)
extern "C" int __cdecl _chvalidator(
        int c,
        int mask
        )
{
        _ASSERTE((unsigned)(c + 1) <= 256);
        return _chvalidator_l(NULL, c, mask);
}

extern "C" int __cdecl _chvalidator_l(
        _locale_t plocinfo,
        int c,
        int mask
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    _ASSERTE((unsigned)(c + 1) <= 256);
    if (c >= -1 && c <= 255)
    {
        return (_loc_update.GetLocaleT()->locinfo->pctype[c] & mask);
    }
    else
    {
        return (_loc_update.GetLocaleT()->locinfo->pctype[-1] & mask);
    }
}

#endif  /* defined (_DEBUG) */

/***
*_isctype - support is* ctype functions/macros for two-byte multibyte chars
*
*Purpose:
*       This function is called by the is* ctype functions/macros
*       (e.g. isalpha()) when their argument is a two-byte multibyte char.
*       Returns true or false depending on whether the argument satisfies
*       the character class property encoded by the mask.
*
*Entry:
*       int c - the multibyte character whose type is to be tested
*       unsigned int mask - the mask used by the is* functions/macros
*                  corresponding to each character class property
*
*       The leadbyte and the trailbyte should be packed into the int c as:
*
*       H.......|.......|.......|.......L
*           0       0   leadbyte trailbyte
*
*Exit:
*       Returns non-zero if c is of the character class.
*       Returns 0 if c is not of the character class.
*
*Exceptions:
*       Returns 0 on any error.
*
*******************************************************************************/

extern "C" int __cdecl _isctype_l (
        int c,
        int mask,
        _locale_t plocinfo
        )
{
        int size;
        unsigned short chartype;
        char buffer[3];
        _LocaleUpdate _loc_update(plocinfo);

        /* c valid between -1 and 255 */
        if ( c >= -1 && c <= 255 )
            return _loc_update.GetLocaleT()->locinfo->pctype[c] & mask;

        if ( _isleadbyte_l(c >> 8 & 0xff, _loc_update.GetLocaleT()) )
        {
            buffer[0] = (c >> 8 & 0xff); /* put lead-byte at start of str */
            buffer[1] = (char)c;
            buffer[2] = 0;
            size = 2;
        } else {
            buffer[0] = (char)c;
            buffer[1] = 0;
            size = 1;
        }

        if ( 0 == __crtGetStringTypeA(
                    _loc_update.GetLocaleT(),
                    CT_CTYPE1,
                    buffer,
                    size,
                    &chartype,
                    _loc_update.GetLocaleT()->locinfo->lc_codepage,
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE],
                    TRUE) )
        {
            return 0;
        }

        return (int)(chartype & mask);
}

extern "C" int __cdecl _isctype (
        int c,
        int mask
        )
{
    if (__locale_changed == 0)
    {
        return __initiallocinfo.pctype[c] & mask;
    }
    else
    {
        return _isctype_l(c, mask, NULL);
    }
}

