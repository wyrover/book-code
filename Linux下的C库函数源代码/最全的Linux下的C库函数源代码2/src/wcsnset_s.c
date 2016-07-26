/***
*wcsnset_s.c - contains wcsnset_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   wcsnset_s() sets at most the first n characters of a string
*   to a given character; ensure null termination.
*
*******************************************************************************/


#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _wcsnset_s
#define _CHAR wchar_t
#define _CHAR_INT wchar_t
#define _DEST _Dst
#define _SIZE _SizeInWords
#define _COUNT _Count

#include <tcsnset_s.inl>

