/***
*wcsncat_s.c - append n chars of string to new string
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines wcsncat_s() - appends n characters of string onto
*   end of other string
*
*******************************************************************************/


#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME wcsncat_s
#define _CHAR wchar_t
#define _DEST _Dst
#define _SIZE _SizeInWords
#define _SRC _Src
#define _COUNT _Count

#include <tcsncat_s.inl>

