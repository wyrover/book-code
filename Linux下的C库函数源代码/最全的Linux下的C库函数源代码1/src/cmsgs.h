/***
*cmsgs.h - runtime errors
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The file defines, in one place, all error message strings used within
*       the C run-time library.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_CMSGS
#define _INC_CMSGS

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

/*
 * runtime error and termination messages
 */

#define EOL "\r\n"

#define _RT_STACK_TXT      "R6000" EOL "- stack overflow" EOL

#define _RT_FLOAT_TXT      "R6002" EOL "- floating point not loaded" EOL

#define _RT_INTDIV_TXT     "R6003" EOL "- integer divide by 0" EOL

#define _RT_SPACEARG_TXT   "R6008" EOL "- not enough space for arguments" EOL

#define _RT_SPACEENV_TXT   "R6009" EOL "- not enough space for environment" EOL

#define _RT_ABORT_TXT      "" EOL "This application has requested the Runtime to terminate it in an unusual way.\nPlease contact the application's support team for more information." EOL

#define _RT_THREAD_TXT     "R6016" EOL "- not enough space for thread data" EOL

#define _RT_LOCK_TXT       "R6017" EOL "- unexpected multithread lock error" EOL

#define _RT_HEAP_TXT       "R6018" EOL "- unexpected heap error" EOL

#define _RT_OPENCON_TXT    "R6019" EOL "- unable to open console device" EOL

#define _RT_NONCONT_TXT    "R6022" EOL "- non-continuable exception" EOL

#define _RT_INVALDISP_TXT  "R6023" EOL "- invalid exception disposition" EOL

/*
 * _RT_ONEXIT_TXT is specific to Win32 and Dosx32 platforms
 */
#define _RT_ONEXIT_TXT     "R6024" EOL "- not enough space for _onexit/atexit table" EOL

#define _RT_PUREVIRT_TXT   "R6025" EOL "- pure virtual function call" EOL

#define _RT_STDIOINIT_TXT  "R6026" EOL "- not enough space for stdio initialization" EOL

#define _RT_LOWIOINIT_TXT  "R6027" EOL "- not enough space for lowio initialization" EOL

#define _RT_HEAPINIT_TXT   "R6028" EOL "- unable to initialize heap" EOL

#define _RT_CRT_NOTINIT_TXT "R6030" EOL "- CRT not initialized" EOL

#define _RT_CRT_INIT_CONFLICT_TXT "R6031" EOL "- Attempt to initialize the CRT more than once.\n" \
    "This indicates a bug in your application." EOL

#define _RT_LOCALE_TXT "R6032" EOL "- not enough space for locale information" EOL

#define _RT_CRT_INIT_MANAGED_CONFLICT_TXT "R6033" EOL "- Attempt to use MSIL code from this assembly during native code initialization\n" \
    "This indicates a bug in your application. It is most likely the result of calling an MSIL-compiled (/clr) function from a native constructor or from DllMain." EOL

#ifndef _DEBUG
#define _RT_CHECKMANIFEST_TXT "R6034" EOL "An application has made an attempt to load the C runtime library incorrectly.\nPlease contact the application's support team for more information." EOL
#else  /* _DEBUG */
#define _RT_CHECKMANIFEST_TXT "R6034" EOL "An application has made an attempt to load the C runtime library without using a manifest.\n" \
    "This is an unsupported way to load Visual C++ DLLs. You need to modify your application to build with a manifest.\n" \
    "For more information, see the \"Visual C++ Libraries as Shared Side-by-Side Assemblies\" topic in the product documentation." EOL
#endif  /* _DEBUG */

/*
 * _RT_COOKIE_INIT_TXT is used directly as the argument to FatalAppExitA, since it is
 * used in __security_init_cookie when __crtMessageBox may not be available yet.
 */
#define _RT_COOKIE_INIT_TXT "Microsoft Visual C++ Runtime Library, Error R6035 - " \
                            "A module in this application is initializing the module's " \
                            "global security cookie while a function relying on that " \
                            "security cookie is active.  Call __security_init_cookie earlier."

/*
 * _RT_DOMAIN_TXT, _RT_SING_TXT and _RT_TLOSS_TXT are used by the floating
 * point library.
 */
#define _RT_DOMAIN_TXT     "DOMAIN error" EOL

#define _RT_SING_TXT       "SING error" EOL

#define _RT_TLOSS_TXT      "TLOSS error" EOL


#define _RT_CRNL_TXT       EOL

#define _RT_BANNER_TXT     "runtime error "


#endif  /* _INC_CMSGS */
