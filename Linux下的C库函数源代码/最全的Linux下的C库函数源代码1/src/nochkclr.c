/***
* nochkclr.c - Dummy non-version-checking CLR call
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#ifdef _M_IX86

/***
*_check_commonlanguageruntime_version
*
*Purpose:
*       If you don't link to the CRT, you use this obj to fill the compiler's need for this symbol
*
*Entry:
*       Never.
*
*Exit:
*       None
*
*Exceptions:
*       None.
*
*******************************************************************************/

void _check_commonlanguageruntime_version()
{
}

#endif  /* _M_IX86 */
