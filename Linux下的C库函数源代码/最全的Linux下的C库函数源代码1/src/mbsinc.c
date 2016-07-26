/***
*mbsinc.c - Move MBCS string pointer ahead one charcter.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Move MBCS string pointer ahead one character.
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <mbdata.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stddef.h>
#include <internal.h>

/***
*_mbsinc - Move MBCS string pointer ahead one charcter.
*
*Purpose:
*       Move the supplied string pointer ahead by one
*       character.  MBCS characters are handled correctly.
*
*Entry:
*       const unsigned char *current = current char pointer (legal MBCS boundary)
*
*Exit:
*       Returns pointer after moving it.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

unsigned char * __cdecl _mbsinc_l(
        const unsigned char *current,
        _locale_t plocinfo
        )
{
        if ( (_ismbblead_l)(*(current++),plocinfo))
        {
            /* don't move forward two if we get leadbyte, EOS
               also don't assert here as we are too low level
            */
            if(*current!='\0')
            {
                current++;
            }
        }

        return (unsigned char *)current;
}

unsigned char * (__cdecl _mbsinc)(
        const unsigned char *current
        )
{
        /* validation section */
        _VALIDATE_RETURN(current != NULL, EINVAL, NULL);

        if ( _ismbblead(*(current++)))
        {
            /* don't move forward two if we get leadbyte, EOS
               also don't assert here as we are too low level
            */
            if(*current!='\0')
            {
                current++;
            }
        }

        return (unsigned char *)current;
}

#endif  /* _MBCS */
