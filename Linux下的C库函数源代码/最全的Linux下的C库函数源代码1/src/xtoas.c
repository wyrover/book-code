/***
*xtoas.c - Secure Version of xtoa.c
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module is split from xtoa to avoid xtoa.obj pulling in startup code
*
*******************************************************************************/

#define _SECURE_ITOA
#include "xtoa.c"
