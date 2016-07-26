/***
*wcstok_s.c - tokenize a wide-character string with given delimiters
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines wcstok_s() - breaks wide-character string into series of token
*   via repeated calls.
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME wcstok_s
#define _CHAR wchar_t

#include <tcstok_s.inl>
