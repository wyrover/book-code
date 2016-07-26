/* _Stof function */
#include <stdlib.h>
#include "xmath.h"
#include "xxcctype.h"
#include "xxfftype.h"
_C_STD_BEGIN

#if defined(__cplusplus) && !defined(MRTDLL)
extern "C"
{
#endif
_MRTIMP2_NCEEPURE FTYPE __CLRCALL_PURE_OR_CDECL FNAME(Dtentox)(FTYPE, long, int *);

_MRTIMP2_NCEEPURE double __CLRCALL_PURE_OR_CDECL _Stodx(const char *s, char **endptr, long pten, int *perr);

_MRTIMP2_NCEEPURE double __CLRCALL_PURE_OR_CDECL _Stod(const char *s, char **endptr, long pten);

/* keep this for bin compat */
_MRTIMP2_NCEEPURE FTYPE __CLRCALL_PURE_OR_CDECL __Stofx(const CTYPE *s, CTYPE **endptr, long pten, int *perr)
	#include "xxstod.h"

_MRTIMP2_NCEEPURE float __CLRCALL_PURE_OR_CDECL _Stofx(const char *s, char **endptr, long pten, int *perr)
	{	/* convert string to float */
	return ((float)_Stodx(s, endptr, pten, perr));
	}

_MRTIMP2_NCEEPURE float __CLRCALL_PURE_OR_CDECL _Stof(const char *s, char **endptr, long pten, int *perr)
	{	/* convert string to float, discard error code */
	return ((float)_Stod(s, endptr, pten));
	}

#if defined(__cplusplus) && !defined(MRTDLL)
}
#endif

_C_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
