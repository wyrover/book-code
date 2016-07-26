/***
*tolower.c - convert character to lower case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines function versions of _tolower() and tolower().
*
*******************************************************************************/

#include <cruntime.h>
#include <stddef.h>
#include <locale.h>
#include <awint.h>
#include <ctype.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/* remove macro defintions of _tolower() and tolower()
 */
#undef  _tolower
#undef  tolower

/* define function-like macro equivalent to _tolower()
 */
#define mklower(c)  ( (c)-'A'+'a' )

/***
*int _tolower(c) - convert character to lower case
*
*Purpose:
*       _tolower() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       returns int value of lower case representation of c
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _tolower (
        int c
        )
{
        return(mklower(c));
}

/***
*int _tolower_l(int c, _locale_t _loc_update.GetLocaleT()) - convert character to lower case
*
*Purpose:
*       Multi-thread function only!
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _tolower_l (
        int c,
        _locale_t plocinfo
        )
{
    int size;
    unsigned char inbuffer[3];
    unsigned char outbuffer[3];
    _LocaleUpdate _loc_update(plocinfo);

    /* if checking case of c does not require API call, do it */
    if ( (unsigned)c < 256 )
    {
        if (_isupper_l(c, _loc_update.GetLocaleT()))
            return _loc_update.GetLocaleT()->locinfo->pclmap[c];
        else
            return c;
    }

    /* convert int c to multibyte string */
    if (_loc_update.GetLocaleT()->locinfo->mb_cur_max > 1 && _isleadbyte_l(c >> 8 & 0xff, _loc_update.GetLocaleT()) )
    {
        inbuffer[0] = (c >> 8 & 0xff); /* put lead-byte at start of str */
        inbuffer[1] = (unsigned char)c;
        inbuffer[2] = 0;
        size = 2;
    } else {
        /* this is an undefined behavior, should probably use towlower instead */
        errno = EILSEQ;
        inbuffer[0] = (unsigned char)c;
        inbuffer[1] = 0;
        size = 1;
    }

    /* convert to lowercase */
    if ( 0 == (size = __crtLCMapStringA(
                    _loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE],
                    LCMAP_LOWERCASE,
                    (LPCSTR)inbuffer,
                    size,
                    (LPSTR)outbuffer,
                    3,
                    _loc_update.GetLocaleT()->locinfo->lc_codepage,
                    TRUE)) )
    {
        return c;
    }

    /* construct integer return value */
    if (size == 1)
        return ((int)outbuffer[0]);
    else
        return ((int)outbuffer[1] | ((int)outbuffer[0] << 8));
}

/***
*int tolower(c) - convert character to lower case
*
*Purpose:
*       tolower() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       if c is an upper case letter, returns int value of lower case
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/


extern "C" int __cdecl tolower (
        int c
        )
{

    if (__locale_changed == 0)
    {
        return __ascii_towlower(c);
    }
    else
    {
        return _tolower_l(c, NULL);
    }
}

