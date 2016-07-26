/***
*wcsset_s.c - contains wcsset_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   wcsset_s() sets all of the characters in a string equal to a given character.
*
*******************************************************************************/


#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _wcsset_s
#define _CHAR wchar_t
#define _CHAR_INT wchar_t
#define _DEST _Dst
#define _SIZE _SizeInWords

#include <tcsset_s.inl>

