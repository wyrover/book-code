/***
*splitpath_s.c - break down path name into components
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
#define _FUNC_NAME _splitpath_s
#define _CHAR char
#define _TCSNCPY_S strncpy_s
#define _T(_Character) _Character

#define _MBS_SUPPORT 1
/* _splitpath uses _ismbblead and not _ismbblead_l */
#undef _ISMBBLEAD
#define _ISMBBLEAD(_Character) \
    _ismbblead((_Character))

#include <tsplitpath_s.inl>
