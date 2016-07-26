/* _LDunscale function -- IEEE 754 version */
#include "xmath.h"
_C_STD_BEGIN

 #if _DLONG == 0
_MRTIMP2_NCEEPURE short (__CLRCALL_PURE_OR_CDECL _LDunscale)(short *pex, long double *px)
	{	/* separate *px to 1/2 <= |frac| < 1 and 2^*pex -- 64-bit */
	return (_Dunscale(pex, (double *)px));
	}

 #elif _DLONG == 1
_MRTIMP2_NCEEPURE short (__CLRCALL_PURE_OR_CDECL _LDunscale)(short *pex, long double *px)
	{	/* separate *px to 1/2 <= |frac| < 1 and 2^*pex -- 80-bit */
	unsigned short *ps = (unsigned short *)(char *)px;
	short xchar = ps[_L0] & _LMASK;

	if (xchar == _LMAX)
		{	/* NaN or INF */
		*pex = 0;
		return ((ps[_L1] & 0x7fff) != 0 || ps[_L2] != 0
			|| ps[_L3] != 0 || ps[_L4] != 0 ? _NANCODE : _INFCODE);
		}
	else if (ps[_L1] != 0 || ps[_L2] != 0
		|| ps[_L3] != 0 || ps[_L4] != 0)
		{	/* finite, reduce to [1/2, 1) */
		if (xchar == 0)
			xchar = 1;	/* correct denormalized exponent */
		xchar += _LDnorm(ps);
		ps[_L0] = ps[_L0] & _LSIGN | _LBIAS;
		*pex = xchar - _LBIAS;
		return (_FINITE);
		}
	else
		{	/* zero */
		*pex = 0;
		return (0);
		}
	}

 #else	/* 1 < _DLONG */
_MRTIMP2_NCEEPURE short (__CLRCALL_PURE_OR_CDECL _LDunscale)(short *pex, long double *px)
	{	/* separate *px to 1/2 <= |frac| < 1 and 2^*pex -- 128-bit SPARC */
	unsigned short *ps = (unsigned short *)(char *)px;
	short xchar = ps[_L0] & _LMASK;

	if (xchar == _LMAX)
		{	/* NaN or INF */
		*pex = 0;
		return (ps[_L1] != 0 || ps[_L2] != 0 || ps[_L3] != 0
			|| ps[_L4] != 0 || ps[_L5] != 0 || ps[_L6] != 0
			|| ps[_L7] != 0 ? _NANCODE : _INFCODE);
		}
	else if (0 < xchar || (xchar = _LDnorm(ps)) <= 0)
		{	/* finite, reduce to [1/2, 1) */
		ps[_L0] = ps[_L0] & _LSIGN | _LBIAS;
		*pex = xchar - _LBIAS;
		return (_FINITE);
		}
	else
		{	/* zero */
		*pex = 0;
		return (0);
		}
	}
 #endif /* _DLONG */

_C_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
