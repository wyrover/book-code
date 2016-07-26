// clog -- initialize standard log stream
#include <fstream>
#include <iostream>

#ifndef MRTDLL
#pragma warning(disable: 4074)
#pragma init_seg(compiler)
static std::_Init_locks  initlocks;
#endif

_STD_BEGIN
		// OBJECT DECLARATIONS

__PURE_APPDOMAIN_GLOBAL static filebuf flog(_cpp_stderr);
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 ostream clog(&flog);
_MRTIMP2 ostream& __cdecl _Clog_func()
{
    return clog;
}

		// INITIALIZATION CODE
struct _Init_clog
	{	// ensures that clog is initialized
	__CLR_OR_THIS_CALL _Init_clog()
		{	// initialize clog
		_Ptr_clog = &clog;
		clog.tie(_Ptr_cout);
		}
	};
__PURE_APPDOMAIN_GLOBAL static _Init_clog init_clog;

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
