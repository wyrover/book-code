/***
*strset_s.c - contains strset_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   strset_s() sets all of the characters in a string equal to a given character.
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _strset_s
#define _CHAR char
#define _CHAR_INT int
#define _DEST _Dst
#define _SIZE _SizeInBytes

#include <tcsset_s.inl>
