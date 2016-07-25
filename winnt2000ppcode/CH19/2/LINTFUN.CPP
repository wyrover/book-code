/* lintfun.cpp : LargeInt function implementation
   ---------------------------------------------- */
#include "largeint.h"

/* Compute the greatest integer less than or equal
   to the square root of lint.  From "Factorization
   and Primality Testing", Bressoud
   Copyright (C) 1994 John K. Gotwals
   ------------------------------------------------ */
LargeInt sqrt(const LargeInt& lint) {
	LargeInt a = lint;
	LargeInt b = (lint + one) / two;

#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals")

	while (b < a) {
		a = b;
		b = (a * a + lint) / (two * a);
	}
	return a;
}

