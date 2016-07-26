/***
*winput.c - wscanf style input from a FILE (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the symbol UNICODE and then #includes the file
*       "input.c" in order to implement _winput(), the helper for the
*       wide character versions of the *wscanf() family of functions.
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#include "input.c"

