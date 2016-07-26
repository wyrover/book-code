/***
*fgetwchr.c - get a wide character from stdin
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _fgetwchar() and getwchar() - read a wide character from stdin
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <tchar.h>

/***
*wint_t _fgetwchar(), getwchar() - read a wide character from stdin
*
*Purpose:
*       Reads the next wide character from stdin.  Function version of
*       getwchar() macro.
*
*Entry:
*       None.
*
*Exit:
*       Returns wide character read or WEOF if at end-of-file or an error occured,
*       in which case the appropriate flag is set in the FILE structure.
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl _fgetwchar (
        void
        )
{
        return(getwc(stdin));
}

#undef getwchar

wint_t __cdecl getwchar (
        void
        )
{
        return(_fgetwchar());
}

