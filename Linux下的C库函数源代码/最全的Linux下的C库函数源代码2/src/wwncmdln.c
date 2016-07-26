/***
*wwncmdln.c - process command line for wWinMain
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Prepare command line to be passed to wWinMain.
*
*******************************************************************************/


#define WPRFLAG     1

#ifndef _UNICODE
#define _UNICODE    1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE     1
#endif  /* UNICODE */

#undef  _MBCS       /* UNICODE not _MBCS */

#include "wincmdln.c"

