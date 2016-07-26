/***
*fputwchr.c - write a wide character to stdout
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fputwchar(), putwchar() - write a wide character to stdout,
*       function version
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <tchar.h>

/***
*wint_t _fputwchar(ch), putwchar() - put a wide character to stdout
*
*Purpose:
*       Puts the given wide character to stdout.  Function version of macro
*       putwchar().
*
*Entry:
*       wchar_t ch - character to output
*
*Exit:
*       returns character written if successful
*       returns WEOF if fails
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl _fputwchar (
        REG1 wchar_t ch
        )
{
        return(putwc(ch, stdout));
}

#undef putwchar

wint_t __cdecl putwchar (
        REG1 wchar_t ch
        )
{
        return(_fputwchar(ch));
}

