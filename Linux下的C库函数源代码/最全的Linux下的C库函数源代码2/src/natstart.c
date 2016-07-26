/***
*natstart.c - native startup tracking variable.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains native startup tracking variable.
*
*******************************************************************************/

#include <crtdefs.h>
#include <internal.h>

/*
 * __native_startup_state :
 * 0 - No initialization code executed yet.
 * 1 - In process of initialization.
 * 2 - Native initialization done.
 */
_PGLOBAL
#if defined (__cplusplus)
extern "C"
{
#endif  /* defined (__cplusplus) */
volatile unsigned int __native_dllmain_reason = __NO_REASON;
volatile unsigned int __native_vcclrit_reason = __NO_REASON;
volatile __enative_startup_state __native_startup_state; /* process-wide state of native init */
volatile void *__native_startup_lock; /* fiber ID currently doing native init */
#if defined (__cplusplus)
}
#endif  /* defined (__cplusplus) */

