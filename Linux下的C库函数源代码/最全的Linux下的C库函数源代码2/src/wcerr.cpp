// wcerr -- initialize standard wide error stream
#include <fstream>
#include <iostream>

#pragma warning(disable: 4074)
#pragma init_seg(compiler)
static std::_Init_locks initlocks;

_STD_BEGIN
		// OBJECT DECLARATIONS

__PURE_APPDOMAIN_GLOBAL static wfilebuf wferr(_cpp_stderr);
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 wostream wcerr(&wferr);
_MRTIMP2 wostream& __cdecl _Wcerr_func()
{
    return wcerr;
}

		// INITIALIZATION CODE
struct _Init_wcerr
	{	// ensures that wcerr is initialized
	__CLR_OR_THIS_CALL _Init_wcerr()
		{	// initialize wcerr
		_Ptr_wcerr = &wcerr;
		wcerr.tie(_Ptr_wcout);
		wcerr.setf(ios_base::unitbuf);
		}
	};
__PURE_APPDOMAIN_GLOBAL static _Init_wcerr init_wcerr;

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
