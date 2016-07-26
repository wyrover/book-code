/***
*strnset_s.c - contains strnset_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   strnset_s() sets at most the first n characters of a string
*   to a given character; ensure null termination.
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _strnset_s
#define _CHAR char
#define _CHAR_INT int
#define _DEST _Dst
#define _SIZE _SizeInBytes
#define _COUNT _Count

#include <tcsnset_s.inl>
