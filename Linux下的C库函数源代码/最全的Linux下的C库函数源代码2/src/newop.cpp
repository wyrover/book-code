// newop operator new(size_t) for Microsoft C++
#include <cstdlib>
#include <new>

_C_LIB_DECL
int __cdecl _callnewh(size_t count) _THROW1(_STD bad_alloc);
_END_C_LIB_DECL

void *__CRTDECL operator new(size_t count) _THROW1(_STD bad_alloc)
	{	// try to allocate size bytes
	void *p;
	while ((p = malloc(count)) == 0)
		if (_callnewh(count) == 0)
			{	// report no memory
			static const std::bad_alloc nomem;
			_RAISE(nomem);
			}
	return (p);
	}

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
