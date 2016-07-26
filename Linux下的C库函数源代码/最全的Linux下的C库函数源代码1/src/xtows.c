/***
*xtows.c - Secure Version of xtow.c
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module is split from xtow to avoid xtow.obj pulling in startup code
*
*******************************************************************************/

#define _SECURE_ITOA
#define _UNICODE
#include "xtoa.c"
