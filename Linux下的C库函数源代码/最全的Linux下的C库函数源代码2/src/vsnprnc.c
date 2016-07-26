/***
*vsnprnc.c - Version of _vsnprintf with the error return fix.
*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*   The _vsnprintf_c() flavor  returns -1 in case there is no space
*   available for the null terminator & blanks out the buffer
*
*******************************************************************************/


#define _COUNT_ 1
#define _SWPRINTFS_ERROR_RETURN_FIX 1

#include <wchar.h>
#include "vsprintf.c"

