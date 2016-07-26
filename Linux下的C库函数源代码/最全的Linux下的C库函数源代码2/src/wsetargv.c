/***
*wsetargv.c - generic _wsetargv routine (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Linking in this module replaces the normal wsetargv with the
*       wildcard wsetargv.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>

/***
*_wsetargv - sets wargv by calling __wsetargv
*
*Purpose:
*       Routine directly transfers to __wsetargv.
*
*Entry:
*       See __wsetargv.
*
*Exit:
*       See __wsetargv.
*
*Exceptions:
*       See __wsetargv.
*
*******************************************************************************/

int __CRTDECL  _wsetargv (
        void
        )
{
        return __wsetargv();
}
