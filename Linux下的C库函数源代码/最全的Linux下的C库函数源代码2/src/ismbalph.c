/***
*ismbalph.c - Test if character is alphabetic (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Test if character is alphabetic (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <windows.h>
#include <awint.h>

#include <cruntime.h>
#include <ctype.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>


/***
* _ismbcalpha - Test if character is alphabetic (MBCS)
*
*Purpose:
*       Test if character is alphabetic.
*       Handles MBCS chars correctly.
*
*       Note:  Use test against 0x00FF instead of _ISLEADBYTE
*       to ensure that we don't call SBCS routine with a two-byte
*       value.
*
*Entry:
*       unsigned int c = character to test
*
*Exit:
*       Returns TRUE if c is alphabetic, else FALSE
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _ismbcalpha_l(
        unsigned int c,
        _locale_t plocinfo
        )
{
        _LocaleUpdate _loc_update(plocinfo);

        if (c > 0x00FF)
        {


            char buf[2];
            unsigned short ctype[2] = {0};

            buf[0] = (c >> 8) & 0xFF;
            buf[1] = c & 0xFF;

            /* return FALSE if not in supported MB code page */
            if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
                return 0;

            /*
            * Since 'c' could be two one-byte MB chars, we need room in the
            * ctype return array to handle this. In this case, the
            * second word in the return array will be non-zero.
            */

            if ( __crtGetStringTypeA( _loc_update.GetLocaleT(),
                                      CT_CTYPE1,
                                      buf,
                                      2,
                                      ctype,
                                      _loc_update.GetLocaleT()->mbcinfo->mbcodepage,
                                      _loc_update.GetLocaleT()->mbcinfo->mblcid,
                                      TRUE ) == 0)
                return 0;

            /* ensure single MB character and test for type */
            return (ctype[1] == 0 && ctype[0] & (_ALPHA));


        } else
        {
            return _ismbbalpha_l( c, _loc_update.GetLocaleT());
        }
}

extern "C" int (__cdecl _ismbcalpha)(
        unsigned int c
        )
{
        return _ismbcalpha_l(c, NULL);
}

#endif  /* _MBCS */
