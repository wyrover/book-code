/***
*fgetchar.c - get a character from stdin
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fgetchar() and getchar() - read a character from stdin
*       defines _fgetwchar() and getwchar() - read a wide character from stdin
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>

/***
*int _fgetchar(), getchar() - read a character from stdin
*
*Purpose:
*       Reads the next character from stdin.  Function version of
*       getchar() macro.
*
*Entry:
*       None.
*
*Exit:
*       Returns character read or EOF if at end-of-file or an error occured,
*       in which case the appropriate flag is set in the FILE structure.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fgetchar (
        void
        )
{
        return(getc(stdin));
}

#undef getchar

int __cdecl getchar (
        void
        )
{
        return _fgetchar();
}
