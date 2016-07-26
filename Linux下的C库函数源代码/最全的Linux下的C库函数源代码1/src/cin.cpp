// cin -- initialize standard input stream
#include <fstream>
#include <iostream>

#pragma warning(disable: 4074)
#pragma init_seg(compiler)
static std::_Init_locks  initlocks;

_STD_BEGIN
		// OBJECT DECLARATIONS

__PURE_APPDOMAIN_GLOBAL static filebuf fin(_cpp_stdin);
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2 istream cin(&fin);
_MRTIMP2 istream& __cdecl _Cin_func()
{
    return cin;
}

		// INITIALIZATION CODE
struct _Init_cin
	{	// ensures that cin is initialized
	__CLR_OR_THIS_CALL _Init_cin()
		{	// initialize cin
		_Ptr_cin = &cin;
		cin.tie(_Ptr_cout);
		}
	};
__PURE_APPDOMAIN_GLOBAL static _Init_cin init_cin;

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
