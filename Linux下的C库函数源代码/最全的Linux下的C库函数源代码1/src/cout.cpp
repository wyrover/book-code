// cout -- initialize standard output stream
#include <fstream>
#include <iostream>

#pragma warning(disable: 4074)
#pragma init_seg(compiler)
static std::_Init_locks  initlocks;

_STD_BEGIN
		// OBJECT DECLARATIONS

__PURE_APPDOMAIN_GLOBAL static filebuf fout(_cpp_stdout);
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 ostream cout(&fout);
_MRTIMP2 ostream& __cdecl _Cout_func()
{
    return cout;
}

		// INITIALIZATION CODE
struct _Init_cout
	{	// ensures that cout is initialized
	__CLR_OR_THIS_CALL _Init_cout()
		{	// initialize cout
		_Ptr_cout = &cout;
		if (_Ptr_cin != 0)
			_Ptr_cin->tie(_Ptr_cout);
		if (_Ptr_cerr != 0)
			_Ptr_cerr->tie(_Ptr_cout);
		if (_Ptr_clog != 0)
			_Ptr_clog->tie(_Ptr_cout);
		}
	};
__PURE_APPDOMAIN_GLOBAL static _Init_cout init_cout;

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
