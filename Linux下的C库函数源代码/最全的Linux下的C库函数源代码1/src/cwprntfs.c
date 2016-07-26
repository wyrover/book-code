/***
*cwprntfs.c - Secure version of cwprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*   This has format validations as compared to cwprintf
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#define FORMAT_VALIDATIONS 1
#define CPRFLAG 1
#include "output.c"

