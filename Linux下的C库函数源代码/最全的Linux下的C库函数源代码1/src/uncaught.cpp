// uncaught -- uncaught_exception for Microsoft

  #include <eh.h>
  #include <exception>
_STD_BEGIN

#if defined(_M_CEE_PURE) || defined(MRTDLL)
_MRTIMP bool __cdecl _uncaught_exception_m()
#else
_CRTIMP2 bool __cdecl uncaught_exception()
#endif
	{	// report if handling a throw
	return (__uncaught_exception());
	}
_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
