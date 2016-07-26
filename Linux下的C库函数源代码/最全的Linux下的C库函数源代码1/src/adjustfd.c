/***
*adjustfd.c - define the compiler-helper variable _adjust_fdiv
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The compiler generates references to this variable.  It must be
*       defined in the local module (EXE or DLL) and is actually imported
*       from MSVCRT*.DLL when this module starts up.
*
*******************************************************************************/

#if defined (CRTDLL)

int _adjust_fdiv;

#endif  /* defined (CRTDLL) */
