/***
*strcpy_s.c - contains strcpy_s()
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   strcpy_s() copies one string onto another.
*
*******************************************************************************/

#include <string.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME strcpy_s
#define _CHAR char
#define _DEST _Dst
#define _SIZE _SizeInBytes
#define _SRC _Src

#include <tcscpy_s.inl>
