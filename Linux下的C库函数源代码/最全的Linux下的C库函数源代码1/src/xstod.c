/* _Stod function */
#include <stdlib.h>
#include "xmath.h"
#include "xxcctype.h"
#include "xxdftype.h"

_C_STD_BEGIN

#if defined(__cplusplus) && !defined(MRTDLL)
extern "C"
#endif
_MRTIMP2_NCEEPURE FTYPE __CLRCALL_PURE_OR_CDECL FNAME(Dtentox)(FTYPE, long, int *);

#if defined(__cplusplus) && !defined(MRTDLL)
extern "C"
#endif
/* keep this for bin compat */
_MRTIMP2_NCEEPURE FTYPE __CLRCALL_PURE_OR_CDECL __Stodx(const CTYPE *s, CTYPE **endptr, long pten, int *perr)
	#include "xxstod.h"

_MRTIMP2_NCEEPURE double __CLRCALL_PURE_OR_CDECL _Stodx(const char *s, char **endptr, long pten, int *perr)
	{	/* convert string to double */
	int err = errno;
	double x = 0.0;
	errno = 0;
	x = strtod(s, endptr);

	*perr = errno;
	errno = err;

	for (; 0 < pten; --pten)
		x *= 10.0;
	for (; pten < 0; ++pten)
		x /= 10.0;
	return (x);
	}

_MRTIMP2_NCEEPURE double __CLRCALL_PURE_OR_CDECL _Stod(const char *s, char **endptr, long pten)
	{	/* convert string, discard error code */
	int err = 0;
	return _Stodx(s, endptr, pten, &err);
	}

_C_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
