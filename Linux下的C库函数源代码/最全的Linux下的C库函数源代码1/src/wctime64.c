/***
*wctime64.c - convert time argument into a string (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _wctime64() - convert time value to wide char string
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "ctime64.c"

