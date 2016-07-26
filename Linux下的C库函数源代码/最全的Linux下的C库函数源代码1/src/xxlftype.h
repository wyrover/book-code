/* xxlftype.h -- parameters for long double floating-point type */
#include <float.h>

#define FTYPE	long double
#define FCOMP	long double
#define FCTYPE	_Lcomplex
#define FBITS	LDBL_MANT_DIG
#define FEPS	LDBL_EPSILON
#define FMAXEXP	LDBL_MAX_EXP
#define FFUN(fun)	fun##l
#define FMACRO(x)	L##x
#define FNAME(fun)	_L##fun
#define FCONST(obj)	_L##obj._Long_double
#define FLIT(lit)	lit##L
#define FISNEG(exp)	LSIGN(exp)
#define FCPTYPE	complex<long double>

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
