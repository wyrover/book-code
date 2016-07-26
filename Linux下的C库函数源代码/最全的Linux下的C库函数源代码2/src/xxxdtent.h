/* xxxdtent.h -- common _[FL]Dtento functionality */
#include "xmath.h"
 #pragma warning(disable: 4701)
_C_STD_BEGIN

		/* static data */
#define NPOWS	(sizeof pows / sizeof pows[0] - 1)

static const FTYPE pows[] = {
	FLIT(1e1),
	FLIT(1e2),
	FLIT(1e4),
	FLIT(1e8),
	FLIT(1e16),
	FLIT(1e32),

 #if 212 <= FMAXEXP
	FLIT(1e64),

  #if 425 <= FMAXEXP
	FLIT(1e128),

   #if 850 <= FMAXEXP
	FLIT(1e256),

    #if 1700 <= FMAXEXP
	FLIT(1e512),

     #if 3401 <= FMAXEXP
	FLIT(1e1024),

      #if 6803 <= FMAXEXP
	FLIT(1e2048),

       #if 13606 <= FMAXEXP
	FLIT(1e4096),
       #endif /* 13606 <= FMAXEXP */

      #endif /* 6803 <= FMAXEXP */
     #endif /* 3401 <= FMAXEXP */

    #endif /* 1700 <= FMAXEXP */
   #endif /* 850 <= FMAXEXP */

  #endif /* 425 <= FMAXEXP */
 #endif /* 212 <= FMAXEXP */

	};
static const size_t npows = NPOWS;

static short dmul(FTYPE *px, FTYPE y)
	{	/* multiply y by *px with checking */
	short xexp;

	FNAME(Dunscale)(&xexp, px);
	*px *= y;
	return (FNAME(Dscale)(px, xexp));
	}

#if defined(__cplusplus) && !defined(MRTDLL)
extern "C"
#endif
_MRTIMP2_NCEEPURE FTYPE __CLRCALL_PURE_OR_CDECL FNAME(Dtentox)(FTYPE x, long n, int *perr)
	{	/* compute x * 10**n */
	FTYPE factor;
	short errx;
	size_t i;

	if (n == 0 || x == FLIT(0.0))
		return (x);
	factor = FLIT(1.0);
	if (n < 0)
		{	/* scale down */
		unsigned long nu = 0 - (unsigned long)n;

		for (i = 0; 0 < nu && i < npows; nu >>= 1, ++i)
			if (nu & 1)
				factor *= *(pows + i);
		errx = dmul(&x, FLIT(1.0) / factor);
		if (errx < 0 && 0 < nu)
			for (factor = FLIT(1.0) / *(pows + npows); 0 < nu; --nu)
				if (0 <= (errx = dmul(&x, factor)))
					break;
		}
	else if (0 < n)
		{	/* scale up */
		for (i = 0; 0 < n && i < npows; n >>= 1, ++i)
			if (n & 1)
				factor *= *(pows + i);

		errx = dmul(&x, factor);
		if (errx < 0 && 0 < n)
			for (factor = *(pows + npows); 0 < n; --n)
				if (0 <= (errx = dmul(&x, factor)))
					break;
		}
	if (errx == 0 || errx == _INFCODE)
		{	/* report error and set errno */
		errno = ERANGE;
		if (perr != 0)
			*perr |= 1;
		}
	return (x);
	}
_C_STD_END

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
