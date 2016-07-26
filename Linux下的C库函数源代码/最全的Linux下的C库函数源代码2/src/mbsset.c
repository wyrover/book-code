/***
*mbsset.c - Sets all charcaters of string to given character (MBCS)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets all charcaters of string to given character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>
#include <crtdbg.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>

/***
* mbsset - Sets all charcaters of string to given character (MBCS)
*
*Purpose:
*       Sets all of characters in string (except the terminating '/0'
*       character) equal to the supplied character.  Handles MBCS
*       chars correctly.
*
*Entry:
*       unsigned char *string = string to modify
*       unsigned int val = value to fill string with
*
*Exit:
*       returns string = now filled with the specified char
*
*Uses:
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" unsigned char * __cdecl _mbsset_l(
        unsigned char *string,
        unsigned int val,
        _locale_t plocinfo
        )
{
        unsigned char  *start = string;
        unsigned char highval, lowval;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(string != NULL, EINVAL, NULL);

_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        if (_loc_update.GetLocaleT()->mbcinfo->ismbcodepage == 0)
                return (unsigned char *)_strset((char *)string, val);
_END_SECURE_CRT_DEPRECATION_DISABLE

        if (highval = (unsigned char) (val>>8)) {

                /* 2-byte value */

                lowval = (unsigned char)(val & 0x00ff);

                if(lowval=='\0')
                {
                    _ASSERTE(("invalid MBCS pair passed to mbsset",0));

                    /* Ideally we would return NULL here and signal an error
                       condition. But since this function has no other
                       error modes, there would be a good chance of crashing
                       the caller. So instead we fill the string with spaces
                       to ensure that no information leaks through
                       unexpectedly. Anyway, we do set errno to EINVAL.
                    */
                    errno = EINVAL;
                    lowval=highval=' ';
                }

                while (*string) {

                        *string++ = highval;
                        if (*string)
                                *string++ = lowval;
                        else
                                /* don't orphan lead byte */
                                string[-1] = ' ';
                        }

        }

        else {
                /* single byte value */

                while (*string)
                        *string++ = (unsigned char)val;
        }

        return(start);
}

extern "C" unsigned char * (__cdecl _mbsset)(
        unsigned char *string,
        unsigned int val
        )
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
    return _mbsset_l(string, val, NULL);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

#endif  /* _MBCS */
