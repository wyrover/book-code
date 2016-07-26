/***
*strcat_s.c - contains strcat_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   strcat_s() concatenates (appends) a copy of the source string to the
*   end of the destination string.
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME strcat_s
#define _CHAR char
#define _DEST _Dst
#define _SIZE _SizeInBytes
#define _SRC _Src

#include <tcscat_s.inl>
