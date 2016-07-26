/***
*wsplitpath_s.c - break down path name into components
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   To provide support for accessing the individual components of an
*   arbitrary path name
*
*******************************************************************************/

#include <stdlib.h>
#include <mbstring.h>
#include <internal_securecrt.h>

#define _FUNC_PROLOGUE
#define _FUNC_NAME _wsplitpath_s
#define _CHAR wchar_t
#define _TCSNCPY_S wcsncpy_s
#define _T(_Character) L##_Character
#define _MBS_SUPPORT 0

#include <tsplitpath_s.inl>
