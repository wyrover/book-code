// ****************************************************************************
//
// Module:  ipcos.h
// Author:  Dick Lam
//
// Purpose: C++ class header file for ipc classes
//
// Notes:  This file contains operating system-specific defines.
//
// ****************************************************************************

#ifndef MODULE_ipcosh
#define MODULE_ipcosh

// define operating system-specific keywords based on Borland C++ for
// OS/2 and Borland C++ 4.0 for Windows

#ifdef __OS2__
#define EXPORT _export
#endif

#ifdef _Windows

#ifdef __DLL__
#define EXPORT _export
#else
#define EXPORT _import
#endif

#endif

#ifdef _AIX
#define EXPORT
#endif

#endif

// ****************************************************************************

// end of ipc.h
