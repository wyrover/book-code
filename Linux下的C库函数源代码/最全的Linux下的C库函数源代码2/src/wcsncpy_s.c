/***
*wcsncpy_s.c - copy at most n characters of wide-character string
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines wcsncpy_s() - copy at most n characters of wchar_t string
*
*******************************************************************************/


#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME wcsncpy_s
#define _CHAR wchar_t
#define _DEST _Dst
#define _SIZE _SizeInWords
#define _SRC _Src
#define _COUNT _Count

#include <tcsncpy_s.inl>

