// string -- template string support functions
#include <istream>
_STD_BEGIN

// We are not using these functions from the mixed msvcm80.dll, because we cannot throw from /clr modules
// and catch from /clr:pure modules. These functions should never be called from msvcm80.dll, so we use _THROW_NCEE,
// which call _invoke_watsonsignaling an internal error in our libraries.
// The functions are actually implemented inline (see xstring and stdexcept).
// See VSW#476338 for details.

_MRTIMP2 void __cdecl _String_base::_Xlen()
	{	// report a length_error
	_THROW_NCEE(length_error, "string too long");
	}

_MRTIMP2 void __cdecl _String_base::_Xran()
	{	// report an out_of_range error
	_THROW_NCEE(out_of_range, "invalid string position");
	}

_MRTIMP2 void __cdecl _String_base::_Xinvarg()
	{	// report an out_of_range error
	_THROW_NCEE(invalid_argument, "invalid string argument");
	}

_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
