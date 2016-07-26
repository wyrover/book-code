/***
*vswprnc.c - Non standard version of swprintf
*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*   The _vswprintf_c() flavor does take a count argument & also
*   returns the correct error value, in case there is no space
*   available for the null terminator
*
*******************************************************************************/


#define _COUNT_ 1
#define _SWPRINTFS_ERROR_RETURN_FIX 1

#include <wchar.h>
#include "vswprint.c"

