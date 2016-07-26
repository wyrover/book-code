/***
*strncpy_s.c - copy at most n characters of string
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines strncpy_s() - copy at most n characters of string
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME strncpy_s
#define _CHAR char
#define _DEST _Dst
#define _SIZE _SizeInBytes
#define _SRC _Src
#define _COUNT _Count

#include <tcsncpy_s.inl>
