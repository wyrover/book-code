// iosptrs -- iostream object pointers for Microsoft
#include <iostream>
_STD_BEGIN

#if defined(_M_CEE) && !defined(_M_CEE_MIXED)
#error This file cannot be built /clr:pure, etc. because of the use of _PGLOBAL.
#endif

#pragma warning(disable:4439)	// C4439: function with a managed parameter must have a __clrcall calling convention

#pragma warning(disable: 4074)
#pragma init_seg(compiler)
_PGLOBAL static std::_Init_locks initlocks;

		// OBJECT DECLARATIONS
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 istream *_Ptr_cin = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 ostream *_Ptr_cout = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 ostream *_Ptr_cerr = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 ostream *_Ptr_clog = 0;

		// WIDE OBJECTS
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wistream *_Ptr_wcin = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wcout = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wcerr = 0;
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream *_Ptr_wclog = 0;
_STD_END

_C_STD_BEGIN
		// FINALIZATION CODE
#define NATS	10	/* fclose, xgetloc, locks, facet free, etc. */

		/* static data */
__PURE_APPDOMAIN_GLOBAL static void (__cdecl* atfuns_cdecl[NATS])(void) = {0};
__PURE_APPDOMAIN_GLOBAL static size_t atcount_cdecl = {NATS};
_MRTIMP2 void __cdecl _Atexit(void (__cdecl *pf)())
	{	// add to wrapup list
	if (atcount_cdecl == 0)
		abort();	/* stack full, give up */
	else
		atfuns_cdecl[--atcount_cdecl] = pf;
	}

struct _Init_atexit
	{	// controller for atexit processing
	__CLR_OR_THIS_CALL ~_Init_atexit()
		{	// process wrapup functions
		while (atcount_cdecl < NATS)
			if (atfuns_cdecl[atcount_cdecl++])
                (*atfuns_cdecl[atcount_cdecl-1])();
		}
	};

__PURE_APPDOMAIN_GLOBAL static _Init_atexit init_atexit;

char _PJP_CPP_Copyright[] =
	"Copyright (c) 1992-2004 by P.J. Plauger,"
	" licensed by Dinkumware, Ltd."
	" ALL RIGHTS RESERVED.";
_C_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
