// nomemory -- report out of memory
#include <new>
#include <yvals.h>
_STD_BEGIN

_MRTIMP2 void __cdecl _Nomemory()
	{	// report out of memory
	static const _XSTD bad_alloc nomem;
	_RAISE(nomem);
	}
_STD_END

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
