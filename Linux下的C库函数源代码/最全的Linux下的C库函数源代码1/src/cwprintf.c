/***
*cwprintf.c - Conio version of wprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Perform formatted i/o directly to the console.
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#define CPRFLAG 1
#include "output.c"

