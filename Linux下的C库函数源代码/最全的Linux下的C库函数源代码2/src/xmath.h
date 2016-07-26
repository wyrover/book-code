/* xmath.h internal header for Microsoft C */
#ifndef _XMATH
#define _XMATH
#include <errno.h>
#include <math.h>
#include <stddef.h>
#ifndef _YMATH
 #include <ymath.h>
#endif

_C_STD_BEGIN

		/* FLOAT PROPERTIES */
#define _DBIAS	0x3fe
#define _DOFF	4
#define _FBIAS	0x7e
#define _FOFF	7
#define _FRND	1

 #define _D0	3	/* little-endian, small long doubles */
 #define _D1	2
 #define _D2	1
 #define _D3	0
 #define _DLONG	0
 #define _LBIAS	0x3fe
 #define _LOFF	4

		/* IEEE 754 double properties */
#define _DFRAC	((unsigned short)((1 << _DOFF) - 1))
#define _DMASK	((unsigned short)(0x7fff & ~_DFRAC))
#define _DMAX	((unsigned short)((1 << (15 - _DOFF)) - 1))
#define _DSIGN	((unsigned short)0x8000)
#define DSIGN(x)	(((unsigned short *)&(x))[_D0] & _DSIGN)
#define HUGE_EXP	(int)(_DMAX * 900L / 1000)
#define HUGE_RAD	2.73e9	/* ~ 2^33 / pi */
#define SAFE_EXP	((unsigned short)(_DMAX >> 1))

		/* IEEE 754 float properties */
#define _FFRAC	((unsigned short)((1 << _FOFF) - 1))
#define _FMASK	((unsigned short)(0x7fff & ~_FFRAC))
#define _FMAX	((unsigned short)((1 << (15 - _FOFF)) - 1))
#define _FSIGN	((unsigned short)0x8000)
#define FSIGN(x)	(((unsigned short *)&(x))[_F0] & _FSIGN)
#define FHUGE_EXP	(int)(_FMAX * 900L / 1000)
#define FHUGE_RAD	31.8	/* ~ 2^10 / pi */
#define FSAFE_EXP	((unsigned short)(_FMAX >> 1))

 #define _F0	1	/* little-endian order */
 #define _F1	0

		/* IEEE 754 long double properties */
#define _LFRAC	((unsigned short)(-1))
#define _LMASK	((unsigned short)0x7fff)
#define _LMAX	((unsigned short)0x7fff)
#define _LSIGN	((unsigned short)0x8000)
#define LSIGN(x)	(((unsigned short *)&(x))[_L0] & _LSIGN)
#define LHUGE_EXP	(int)(_LMAX * 900L / 1000)
#define LHUGE_RAD	2.73e9	/* ~ 2^33 / pi */
#define LSAFE_EXP	((unsigned short)(_LMAX >> 1))

 #define _L0	3	/* little-endian, small long doubles */
 #define _L1	2
 #define _L2	1
 #define _L3	0
 #define _L4	xxx

		/* return values for testing functions */
#define FINITE	_FINITE
#define INF		_INFCODE
#define NAN		_NANCODE

		/* return values for _Stopfx/_Stoflt */
#define FL_ERR	0
#define FL_DEC	1
#define FL_HEX	2
#define FL_INF	3
#define FL_NAN	4
#define FL_NEG	8

#ifndef MRTDLL
#ifndef _M_CEE_PURE
_C_LIB_DECL
#endif
#endif

_MRTIMP2_NCEEPURE int __CLRCALL_PURE_OR_CDECL _Stopfx(const char **, char **);
_MRTIMP2_NCEEPURE int __CLRCALL_PURE_OR_CDECL _Stoflt(const char *, const char *, char **,
	long[], int);
_MRTIMP2_NCEEPURE int __CLRCALL_PURE_OR_CDECL _Stoxflt(const char *, const char *, char **,
	long[], int);
_MRTIMP2 int __cdecl _WStopfx(const wchar_t **, wchar_t **);
_MRTIMP2 int __cdecl _WStoflt(const wchar_t *, const wchar_t *, wchar_t **,
	long[], int);
_MRTIMP2 int __cdecl _WStoxflt(const wchar_t *, const wchar_t *, wchar_t **,
	long[], int);


		/* double declarations */
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Dnorm(unsigned short *);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Dscale(double *, long);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _Dunscale(short *, double *);
_MRTIMP2_NCEEPURE double __CLRCALL_PURE_OR_CDECL _Poly(double, const double *, int);

extern _CRTIMP2_PURE /* const */ _Dconst _Eps, _Rteps;
extern _CRTIMP2_PURE /* const */ double _Xbig;

		/* float declarations */
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _FDnorm(unsigned short *);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _FDscale(float *, long);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _FDunscale(short *, float *);

extern _CRTIMP2_PURE /* const */ _Dconst _FEps, _FRteps;
extern _CRTIMP2_PURE /* const */ float _FXbig;

		/* long double functions */
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _LDnorm(unsigned short *);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _LDscale(long double *, long);
_MRTIMP2_NCEEPURE short __CLRCALL_PURE_OR_CDECL _LDunscale(short *, long double *);
_MRTIMP2_NCEEPURE long double __CLRCALL_PURE_OR_CDECL _LPoly(long double, const long double *, int);

extern _CRTIMP2_PURE /* const */ _Dconst _LEps, _LRteps;
extern _CRTIMP2_PURE /* const */ long double _LXbig;
#ifndef MRTDLL
#ifndef _M_CEE_PURE
_END_C_LIB_DECL
#endif
#endif
_C_STD_END
#endif /* _XMATH */

/*
 * Copyright (c) 1992-2005 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.05:0009 */
