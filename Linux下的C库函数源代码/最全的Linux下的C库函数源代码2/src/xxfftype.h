/* xxfftype.h -- parameters for float floating-point type */
#include <float.h>

#define FTYPE	float
#define FCOMP	double
#define FCTYPE	_Fcomplex
#define FBITS	FLT_MANT_DIG
#define FEPS	FLT_EPSILON
#define FMAXEXP	FLT_MAX_EXP
#define FFUN(fun)	fun##f
#define FMACRO(x)	F##x
#define FNAME(fun)	_F##fun
#define FCONST(obj)	_F##obj._Float
#define FLIT(lit)	lit##F
#define FISNEG(exp)	FSIGN(exp)
#define FCPTYPE	complex<float>

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
