/* xxdftype.h -- parameters for double floating-point type */
#include <float.h>

#define FTYPE	double
#define FCOMP	double
#define FCTYPE	_Dcomplex
#define FBITS	DBL_MANT_DIG
#define FEPS	DBL_EPSILON
#define FMAXEXP	DBL_MAX_EXP
#define FFUN(fun)	fun
#define FMACRO(x)	x
#define FNAME(fun)	_##fun
#define FCONST(obj)	_##obj._Double
#define FLIT(lit)	lit
#define FISNEG(exp)	DSIGN(exp)
#define FCPTYPE	complex<double>

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
