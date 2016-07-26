/***
*toupper.c - convert character to uppercase
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines function versions of _toupper() and toupper().
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

/* remove macro definitions of _toupper() and toupper()
 */
#undef  _toupper
#undef  toupper

/* define function-like macro equivalent to _toupper()
 */
#define mkupper(c)  ( (c)-'a'+'A' )

/***
*int _toupper(c) - convert character to uppercase
*
*Purpose:
*       _toupper() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       returns int value of uppercase representation of c
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _toupper (
        int c
        )
{
        return(mkupper(c));
}


/***
*int _toupper_l(c, ptloci) - convert character to uppercase
*
*Purpose:
*       Multi-thread function! Non-locking version of toupper.
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/


extern "C" _CRTIMP int __cdecl _toupper_l (
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
        if (_islower_l(c,_loc_update.GetLocaleT()))
            return _loc_update.GetLocaleT()->locinfo->pcumap[c];
        else
            return c;
    }

    /* convert int c to multibyte string */
    if ( _loc_update.GetLocaleT()->locinfo->mb_cur_max > 1 && _isleadbyte_l(c >> 8 & 0xff, _loc_update.GetLocaleT()) )
    {
        inbuffer[0] = (c >> 8 & 0xff); /* put lead-byte at start of str */
        inbuffer[1] = (unsigned char)c;
        inbuffer[2] = 0;
        size = 2;
    } else {
        /* this is an undefined behavior, should probably use towupper instead */
        errno = EILSEQ;
        inbuffer[0] = (unsigned char)c;
        inbuffer[1] = 0;
        size = 1;
    }

    /* convert wide char to lowercase */
    if ( 0 == (size = __crtLCMapStringA(
                    _loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE],
                    LCMAP_UPPERCASE,
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
*int toupper(c) - convert character to uppercase
*
*Purpose:
*       toupper() is simply a function version of the macro of the same name.
*
*Entry:
*       c - int value of character to be converted
*
*Exit:
*       if c is a lower case letter, returns int value of uppercase
*       representation of c. otherwise, it returns c.
*
*Exceptions:
*
*******************************************************************************/


extern "C" int __cdecl toupper (
    int c
    )
{
    if (__locale_changed == 0)
{
        return __ascii_towupper(c);
    }
    else
    {
        return _toupper_l(c, NULL);
    }
}
