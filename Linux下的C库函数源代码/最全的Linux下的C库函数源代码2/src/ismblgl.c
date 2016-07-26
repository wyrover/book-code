/***
*ismblgl.c - Tests to see if a given character is a legal MBCS char.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Tests to see if a given character is a legal MBCS character.
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <locale.h>
#include <setlocal.h>


/***
*int _ismbclegal(c) - tests for a valid MBCS character.
*
*Purpose:
*       Tests to see if a given character is a legal MBCS character.
*
*Entry:
*       unsigned int c - character to test
*
*Exit:
*       returns non-zero if Microsoft Kanji code, else 0
*
*Exceptions:
*
******************************************************************************/

extern "C" int __cdecl _ismbclegal_l(
        unsigned int c,
        _locale_t plocinfo
        )
{
        _LocaleUpdate _loc_update(plocinfo);

        return( (_ismbblead_l(c >> 8, _loc_update.GetLocaleT())) &&
                (_ismbbtrail_l(c & 0x0ff, _loc_update.GetLocaleT())) );
}
extern "C" int (__cdecl _ismbclegal)(
        unsigned int c
        )
{
    return _ismbclegal_l(c, NULL);
}

#endif  /* _MBCS */
