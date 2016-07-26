/***
*nlsdata3.c - globals for international library - locale id's
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the definition of locale id's.  These id's and
*       this file should only be visible to the _init_(locale category)
*       functions.  This module is separated from nlsdatax.c for granularity.
*
*******************************************************************************/

#include <locale.h>
#include <setlocal.h>

/*
 *  Locale id's.
 */
LC_ID __lc_id[LC_MAX-LC_MIN+1] = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
};
