/***
*wcsset.c - sets all characters of wchar_t string to given character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wcsset() - sets all of the characters in a string (except
*       the L'\0') equal to a given character (wide-characters).
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>

#if defined (_M_IA64) || defined (_M_AMD64) || defined (_M_IX86)
#pragma warning(disable:4163)
#pragma function(_wcsset)
#endif  /* defined (_M_IA64) || defined (_M_AMD64) || defined (_M_IX86) */

/***
*wchar_t *_wcsset(string, val) - sets all of string to val (wide-characters)
*
*Purpose:
*       Sets all of wchar_t characters in string (except the terminating '/0'
*       character) equal to val (wide-characters).
*
*
*Entry:
*       wchar_t *string - string to modify
*       wchar_t val - value to fill string with
*
*Exit:
*       returns string -- now filled with val's
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

wchar_t * __cdecl _wcsset (
        wchar_t * string,
        wchar_t val
        )
{
        wchar_t *start = string;

        while (*string)
                *string++ = (wchar_t)val;

        return(start);
}

