/***
*fputchar.c - write a character to stdout
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fputchar(), putchar() - write a character to stdout, function version
*       defines _fputwchar(), putwchar() - write a wide character to stdout, function version
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>

/***
*int _fputchar(ch), putchar() - put a character to stdout
*
*Purpose:
*       Puts the given characters to stdout.  Function version of macro
*       putchar().
*
*Entry:
*       int ch - character to output
*
*Exit:
*       returns character written if successful
*       returns EOF if fails
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fputchar (
        REG1 int ch
        )
{
        return(putc(ch, stdout));
}

#undef putchar

int __cdecl putchar (
        int ch
        )
{
        return _fputchar(ch);
}
