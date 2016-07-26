/***
*woutputs.c - Secure version of wprintf & the wprintf family
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   This has format validations as compared to wprintf
*
*******************************************************************************/



#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#define FORMAT_VALIDATIONS 1
#include "output.c"

