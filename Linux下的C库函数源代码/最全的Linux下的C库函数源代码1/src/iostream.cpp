// iostream -- ios::Init members, dummy for MS
#include <iostream>
_STD_BEGIN

		// OBJECT DECLARATIONS
__PURE_APPDOMAIN_GLOBAL int ios_base::Init::_Init_cnt = -1;
int& ios_base::Init::_Init_cnt_func()
{
    return ios_base::Init::_Init_cnt;
}

_CRTIMP2_PURE void __cdecl ios_base::Init::_Init_ctor( ios_base::Init *_This)
	{	// initialize standard streams first time
	if (0 <= _Init_cnt)
		++_Init_cnt;
	else
		_Init_cnt = 1;
	}

_CRTIMP2_PURE void __cdecl ios_base::Init::_Init_dtor(ios_base::Init *_This)
	{	// flush standard streams last time
	if (--_Init_cnt == 0)
		{	// flush standard streams
		if (_Ptr_cerr != 0)
			_Ptr_cerr->flush();
		if (_Ptr_clog != 0)
			_Ptr_clog->flush();
		if (_Ptr_cout != 0)
			_Ptr_cout->flush();
		}
	}
_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
