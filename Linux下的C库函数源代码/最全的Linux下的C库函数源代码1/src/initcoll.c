/***
*initcoll.c - contains __init_collate
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the locale-category initialization function: __init_collate().
*
*       Each initialization function sets up locale-specific information
*       for their category, for use by functions which are affected by
*       their locale category.
*
*       *** For internal use by setlocale() only ***
*
*******************************************************************************/

#include <windows.h>
#include <locale.h>
#include <setlocal.h>

/***
*int __init_collate() - initialization for LC_COLLATE locale category.
*
*Purpose:
*       The LC_COLLATE category currently requires no initialization.
*
*Entry:
*       None.
*
*Exit:
*       0 success
*       1 fail
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __init_collate (
        pthreadlocinfo ploci
        )
{
        return 0;
}
