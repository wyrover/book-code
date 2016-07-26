/***
*mbbtype.c - Return type of byte based on previous byte (MBCS)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Return type of byte based on previous byte (MBCS)
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
*int _mbbtype(c, ctype) - Return type of byte based on previous byte (MBCS)
*
*Purpose:
*       Returns type of supplied byte.  This decision is context
*       sensitive so a control test condition is supplied.  Normally,
*       this is the type of the previous byte in the string.
*
*Entry:
*       unsigned char c = character to be checked
*       int ctype = control test condition (i.e., type of previous char)
*
*Exit:
*       _MBC_LEAD      = if 1st byte of MBCS char
*       _MBC_TRAIL     = if 2nd byte of MBCS char
*       _MBC_SINGLE    = valid single byte char
*
*       _MBC_ILLEGAL   = if illegal char
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _mbbtype_l(
        unsigned char c,
        int ctype,
        _locale_t plocinfo
        )
{
        _LocaleUpdate _loc_update(plocinfo);

        switch(ctype) {

            case(_MBC_LEAD):
                if ( _ismbbtrail_l(c, _loc_update.GetLocaleT()) )
                    return(_MBC_TRAIL);
                else
                    return(_MBC_ILLEGAL);

            case(_MBC_TRAIL):
            case(_MBC_SINGLE):
            case(_MBC_ILLEGAL):
            default:
                if ( _ismbblead_l(c, _loc_update.GetLocaleT()) )
                    return(_MBC_LEAD);
                else if (_ismbbprint_l( c, _loc_update.GetLocaleT()))
                    return(_MBC_SINGLE);
                else
                    return(_MBC_ILLEGAL);

        }

}

extern "C" int (__cdecl _mbbtype)(
        unsigned char c,
        int ctype
        )
{
        return( _mbbtype_l(c, ctype, NULL) );
}

#endif  /* _MBCS */
