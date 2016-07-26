/* _FDtest function -- IEEE 754 version */
#include "xmath.h"
_C_STD_BEGIN

_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _FDtest(float *px)
	{	/* categorize *px */
	unsigned short *ps = (unsigned short *)(char *)px;

	if ((ps[_F0] & _FMASK) == _FMAX << _FOFF)
		return ((short)((ps[_F0] & _FFRAC) != 0 || ps[_F1] != 0
			? _NANCODE : _INFCODE));
	else if ((ps[_F0] & ~_FSIGN) != 0 || ps[_F1] != 0)
		return ((ps[_F0] & _FMASK) == 0 ? _DENORM : _FINITE);
	else
		return (0);
	}
_C_STD_END

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
