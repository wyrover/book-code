/***
*mbspbrk.c - Find first string char in charset, pointer return (MBCS)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Find first string char in charset, pointer return (MBCS)
*       Shares common source file with mbscspn.c.
*
*******************************************************************************/

#ifdef _MBCS
#define _RETURN_PTR
#include "mbscspn.c"
#endif  /* _MBCS */
