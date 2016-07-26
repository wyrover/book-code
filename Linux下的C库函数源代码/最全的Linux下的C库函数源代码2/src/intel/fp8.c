/***
* fp8.c - Set default FP precision to 53 bits (8-byte double)
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/
#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#include <stddef.h>
#include <float.h>
#include <internal.h>

#ifdef __cplusplus
extern "C"
#endif  /* __cplusplus */
void  __CLRCALL_OR_CDECL _setdefaultprecision(void);

/*
 * Routine to set default FP precision to 53 bits.
 */

#ifdef __cplusplus
extern "C"
#endif  /* __cplusplus */
void __CLRCALL_OR_CDECL _setdefaultprecision()
{
        _ERRCHECK(_controlfp_s(NULL, _PC_53, _MCW_PC));
}
