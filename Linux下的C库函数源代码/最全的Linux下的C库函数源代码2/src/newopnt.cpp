// newopnt operator new(size_t, const nothrow_t&) for Microsoft C++
#ifdef CRTDLL
#undef CRTDLL
#endif

#ifdef MRTDLL
#undef MRTDLL
#endif

#define _USE_ANSI_CPP // suppress defaultlib directive for Std C++ Lib
#include <new>

void * __CRTDECL operator new(size_t count, const std::nothrow_t&)
	_THROW0()
	{	// try to allocate count bytes
	void *p;
	_TRY_BEGIN
	p = operator new(count);
	_CATCH_ALL
	p = 0;
	_CATCH_END
	return (p);
	}

/*
* Copyright (c) 1992-2001 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.10:0009 */
