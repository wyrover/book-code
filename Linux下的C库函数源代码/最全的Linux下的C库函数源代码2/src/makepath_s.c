/***
*makepath_s.c - create path name from components
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   To provide support for creation of full path names from components
*
*******************************************************************************/

#include <stdlib.h>
#include <mbstring.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _makepath_s
#define _CHAR char
#define _DEST _Dst
#define _SIZE _SizeInBytes
#define _T(_Character) _Character

#define _MBS_SUPPORT 1

#include <tmakepath_s.inl>
