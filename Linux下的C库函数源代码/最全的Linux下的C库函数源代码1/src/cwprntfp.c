/***
*cwprntfp.c - Positional version of cwprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*   This has format validations & positional parameters as compared to cwprintf
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#define POSITIONAL_PARAMETERS 1
#define CPRFLAG 1
#include "output.c"

