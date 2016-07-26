/* _Dtest function -- IEEE 754 version */
#include "xmath.h"
_C_STD_BEGIN

_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Dtest(double *px)
	{	/* categorize *px */
	unsigned short *ps = (unsigned short *)(char *)px;

	if ((ps[_D0] & _DMASK) == _DMAX << _DOFF)
		return ((short)((ps[_D0] & _DFRAC) != 0 || ps[_D1] != 0
			|| ps[_D2] != 0 || ps[_D3] != 0 ? _NANCODE : _INFCODE));
	else if ((ps[_D0] & ~_DSIGN) != 0 || ps[_D1] != 0
		|| ps[_D2] != 0 || ps[_D3] != 0)
		return ((ps[_D0] & _DMASK) == 0 ? _DENORM : _FINITE);
	else
		return (0);
	}
_C_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
