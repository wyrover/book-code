/* largeint.cpp : implementation of the LargeInt class
   multiple precision integer arithemtic
   Copyright (C) 1994 John K. Gotwals
   --------------------------------------------------- */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "largeint.h"

int add(const int *u, const int *v, int *w, int n);
void sub(const int *u, const int *v, int *w, int n);
void multiply(const int *u, const int *v, int *w, int n, int m);
void divide(const int *u, const int *v, int *q, int *r, int n, int m);
int divrem(int *u, int v, int m);

#pragma comment(exestr, "For licensing information call (317)494-2564")
#pragma comment(exestr, "THIS IS NOT A PUBLIC DOMAIN PROGRAM\n")
#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals\n")

///////////////////////////////////////////////////////////////////////
// LargeInt construction/destruction

/* LargeInt n1;
   n1 contains 1 radix-2^32 digit of zeros
   ------------------------------------- */
LargeInt::LargeInt() {
	sign = 0;
	len = 1;
	adr = new int[1];
	assert(adr != 0);
	*adr = 0;
}

/* LargeInt n1(30, 0);
   n1 contains 30 radix-2^32 digits of zeros
   ------------------------------ */
LargeInt::LargeInt(int digits, int fill) {
	assert(digits > 0);	// abort on null case
	sign = 0;
	len = digits;
	adr = new int[len];
	assert(adr != 0);
	assert(fill < 256); 
	memset(adr, fill, len * sizeof(int));
}

/* LargeInt n1 = -123456;
   convert int to radix-2^32 int
   must fit in one radix-2^32 int
   ------------------------------ */
LargeInt::LargeInt(int num) {
	len = 1;
	adr = new int[1];
	assert(adr != 0);
	if (num > 0) {
		sign = 1;
		*adr = num;
	}
	else if (num < 0) {
		sign = -1;
		*adr = -num;
	}
	else {
		sign = 0;
		*adr = 0;
	}
}

/* LargeInt n1 = 123456u;
   convert unsigned int to radix-2^32 int
   must fit in one radix-2^32 int
   ------------------------------------ */
LargeInt::LargeInt(unsigned num) {
	if (num == 0)
		sign = 0;
	else
		sign = 1;
	len = 1;
	adr = new int[1];
	assert(adr != 0);
	*adr = num;
}

/* LargeInt n1 = "-123456789...."
   (sign does not have to be present)
   ---------------------------------- */
LargeInt::LargeInt(const char* str) {
	sign = 0;
	len = 1;
	adr = new int[1];
	assert(adr != 0);
	*adr = 0;
	decToBin(str);
}

/* LargeInt n2 = n1;
   copy constructor
   ----------------- */
LargeInt::LargeInt(const LargeInt& lint) {
	sign = lint.sign;
	len  = lint.len;
	adr  = new int[len];
	assert(adr != 0);
	memcpy(adr, lint.adr, len * sizeof(int));
}

//LargeInt::~LargeInt() {
//	delete [] adr;
//}

///////////////////////////////////////////////////////////////////////
// LargeInt operators

/* equality : lint1 == lint2;
   -------------------------- */
int LargeInt::operator==(const LargeInt& lint) const {
	if (sign == 0  && lint.sign == 0)
		return 1; // both operands are zero
	if (len == lint.len      &&
		  sign == lint.sign  &&
		  memcmpInt(adr, lint.adr, len) == 0)
		return 1;
	else
		return 0;
}

/* equality : lint == 123;
   ----------------------- */
int LargeInt::operator==(int num) const {
	LargeInt test;
	test = num;
	return *this == test;
}

/* signed comparison : lint1 < lint2;
   (assumes normalized operands)
   ---------------------------------- */
int LargeInt::operator<(const LargeInt& lint) const {
	if (sign < lint.sign)
		return 1;
	if (sign > lint.sign)
		return 0;
// at this point, the signs are the same
	if (sign == 0)
		return 0;
	if ((sign == -1  &&	 len > lint.len)  ||
		(sign ==  1  &&  len < lint.len))
		return 1;
	if ((sign == -1  &&	 len < lint.len)  ||
		(sign ==  1  &&  len > lint.len))
		return 0;
// at this point, the signs and the lengths are both the same
	int compare = memcmpInt(adr, lint.adr, len);
	if ((sign ==  1  &&  compare < 0)  ||
		(sign == -1  &&  compare > 0))
		return 1;
	return 0;
}

/* signed comparison : lint < 123;
   ------------------------------- */
int LargeInt::operator<(int num) const {
	LargeInt test;
	test = num;
	return *this < test;
}

/* assignment : lint1 = lint2 = lint3;
   ----------------------------------- */
const LargeInt& LargeInt::operator=(const LargeInt& lint) {
	if (this == &lint)	// self assignment test
		return *this;
	delete [] adr;		// get rid of current lint2
	sign = lint.sign;
	len  = lint.len;
	adr  = new int[len];
	assert(adr != 0);
	memcpy(adr, lint.adr, len * sizeof(int));
	return *this;
}

const LargeInt& LargeInt::operator=(int num) {
	if (num > 0)
		sign = 1;
	else if (num == 0)
		sign = 0;
	else
		sign = -1;
	if (len != 1) {
		delete [] adr;
		adr = new int[1];
		assert(adr != 0);
		len = 1;
	}
	*adr = abs(num);
	return *this;
}

const LargeInt& LargeInt::operator=(unsigned num) {
	sign = 1;
	if (len != 1) {
		delete [] adr;
		adr = new int[1];
		assert(adr != 0);
		len = 1;
	}
	*adr = num;
	return *this;
}

const LargeInt& LargeInt::operator=(const char* str) {
	decToBin(str);
	return *this;
}


/* addition : lint3 = lint1 + lint2;
   --------------------------------- */
LargeInt LargeInt::operator+(const LargeInt& lint) const {
	int *u, *v, *w;
	
	// test for either or both operands being 0
	if (sign == 0  &&  lint.sign == 0)
		return zero;
	if (sign == 0)
		return lint;
	if (lint.sign == 0)
		return *this;
	
	// at this point, neither operand is zero
	int n = (len > lint.len)? len : lint.len;
	LargeInt lintx(n, 0); // zero filled for possible copy below

	// make sure both operands have the same number of digits
	if (len == lint.len) {
		u = adr;
		v = lint.adr;
	}
	else if (len > lint.len) {
		u = adr;
		v = lintx.adr;
		lintx.sign = lint.sign;
		memcpy(lintx.adr + (n - lint.len),
			lint.adr, lint.len * sizeof(int));
	}
	else {
		u = lintx.adr;
		v = lint.adr;
		lintx.sign = sign;
		memcpy(lintx.adr + (n - len), adr,
			len * sizeof(int));
	}
	
	if (sign == lint.sign) {
		// create the result assuming no carry from final add
		LargeInt lintw(n, 0);
		lintw.sign = sign;
		w = lintw.adr;
		int carry;
	
		carry = add(u, v, w, n);
		if (carry == 0)
			return lintw;
		else {
			LargeInt lintc(n + 1, 0);
			lintc.sign = sign;
			memcpy(lintc.adr + 1, lintw.adr, n * sizeof(int));
			*lintc.adr = 1; // carry into MSD
			return lintc;
			}
		}
	else { // signs are different
		int cmp;
		LargeInt lintw(n, 0);
		w = lintw.adr;
		cmp = memcmpInt(u, v, n);
		if (cmp < 0) {
			lintw.sign = -sign;
			sub(v, u, w, n);
			}
		else if (cmp > 0) {
			lintw.sign = sign;
			sub(u, v, w, n);
		}
		else {
			return zero;
		}
		lintw.normalize();
		return lintw;
	}
}

/* subtraction : lint3 = lint1 - lint2;
   ------------------------------------ */
LargeInt LargeInt::operator-(const LargeInt& lint) const {
	int *u, *v, *w;
	
	// test for either or both operands being zero
	if (sign == 0  &&  lint.sign == 0)
		return zero;
	if (lint.sign == 0)
		return *this;
	if (sign == 0)
		return - lint;

	// at this point, neither operand is 0
	int n = (len > lint.len)? len : lint.len;
	LargeInt lintx(n, 0); // zero filled for possible copy below

	// make sure both operands have the same number of digits
	if (len == lint.len) {
		u = adr;
		v = lint.adr;
	}
	else if (len > lint.len) {
		u = adr;
		v = lintx.adr;
		lintx.sign = lint.sign;
		memcpy(lintx.adr + (n - lint.len),
			lint.adr, lint.len * sizeof(int));
	}
	else {
		u = lintx.adr;
		v = lint.adr;
		lintx.sign = sign;
		memcpy(lintx.adr + (n - len), adr, len * sizeof(int));
	}
	
	if (sign == lint.sign) {
		// subtract smaller from larger (absolute values)
		int cmp;
		LargeInt lintw(n, 0);
		w = lintw.adr;
		cmp = memcmpInt(u, v, n);
		if (cmp < 0) {
			sub(v, u, w, n);
			lintw.sign = -sign;
		}
		else if (cmp > 0) {
			sub(u, v, w, n);
			lintw.sign = sign;
		}
		else {
			return zero;
		}
		lintw.normalize();
		return lintw;
	}
	else { // signs are different
		// add assuming no carry
		LargeInt lintw(n, 0);
		lintw.sign = sign;
		w = lintw.adr;
		int carry;
	
		carry = add(u, v, w, n);
		if (carry == 0)
			return lintw;
		else {
			LargeInt lintc(n + 1, 0);
			lintc.sign = sign;
			memcpy(lintc.adr + 1, lintw.adr, n * sizeof(int));
			*lintc.adr = 1; // carry into MSD
			return lintc;
		}
	}
}

/* multiplication : lint3 = lint1 * lint2;
   --------------------------------------- */
LargeInt LargeInt::operator*(const LargeInt& lint) const {
	if (sign == 0  || lint.sign == 0)
		return zero;
	LargeInt result(len + lint.len, 0);
	if (sign == lint.sign)
		result.sign = 1;
	else
		result.sign = -1;
	multiply(adr, lint.adr, result.adr, len, lint.len);
	if (*result.adr == 0)
		result.normalize();
	return result;
} 
		
/* multiplication : lint *= num;
   where num is type int
   ----------------------------- */
void LargeInt::operator*=(int num) {
	if (num == 0) {
		*this = zero;
		return;
	}
	LargeInt result(len + 1, 0);
	int absNum = abs(num);
	if ((sign ==  1  &&  num > 0)  |
		(sign == -1	 &&  num < 0))
		result.sign = 1;
	else
		result.sign = -1;
	multiply(adr, &absNum, result.adr, len, 1);
	if (*result.adr == 0)
		result.normalize();
	*this = result;
}

/* divide : lint3 = lint1 / lint2;
   ------------------------------- */
LargeInt LargeInt::operator/(const LargeInt& lint) const {
	// abort if divisor is 0
	if (lint.sign == 0) {
		fprintf(stderr, "\n\aLargeInt / : division by zero\n");
		exit(EXIT_FAILURE);
	}
	// return 0 if dividend is 0
	if (sign == 0)
		return zero;
	
	// operands are normalized and neither is 0
	assert(*adr != 0  &&  *lint.adr != 0);

	// return 0 if dividend < divisor
	if (len < lint.len) // no need to divide
		return zero;
	
	// at this point neither operand is zero
	LargeInt result(len - lint.len + 1, 0);
	if (sign == lint.sign)
		result.sign = 1;
	else
		result.sign = -1;
	divide(adr, lint.adr, result.adr, 0, lint.len, len - lint.len);
	if (*result.adr == 0)
		result.normalize();
	return result;
}

/* modulus or "remainder from division" : lint1 = lint2 % lint3;
   lint1 will always have the sign of lint2
   ------------------------------------------------------------- */
LargeInt LargeInt::operator%(const LargeInt& lint) const {
	// abort if divisor is 0
	if (lint.sign == 0) {
		fprintf(stderr, "\n\aLargeInt %% : division by zero\n");
		exit(EXIT_FAILURE);
	}
	// return 0 if dividend is 0
	if (sign == 0)
		return zero;
	
	// operands are normalized and neither is 0
	assert(*adr != 0  &&  *lint.adr != 0);

	if (len < lint.len) // no need to divide
		return *this;
	LargeInt quotient(len - lint.len + 1, 0);
	LargeInt result(lint.len, 0);
	result.sign = sign;
	divide(adr, lint.adr, quotient.adr, result.adr, lint.len,
			len - lint.len);
	if (*result.adr == 0)
		result.normalize();
	return result;
}

/* friend divRem() - calculates both quotient and remainder	from u/v
   lintq = divRem(lintu, lintv, &lintr);
   lintr will have the same sign as lintu
   lintq will be >0 if lintu and lintv have the same sign else <0
   ----------------------------------------------------------------- */
LargeInt divRem(const LargeInt& u, const LargeInt& v, LargeInt* r) {
	// abort if divisor is 0
	if (v.sign == 0) {
		fprintf(stderr, "\n\aLargeInt divRem() : division by zero\n");
		exit(EXIT_FAILURE);
	}
   	
   	// return 0 for both quotient and remainder if dividend is 0
	if (u.sign == 0) {
		*r = zero;
		return zero;
	}
	
	// operands are normalized and neither is 0
	assert(*u.adr != 0  &&  *v.adr != 0);

	// no need to divide if dividend < divisor
	if (u.len < v.len) {
		*r = u;			// remainder = dividend
		return zero;	// quotient  = zero
	}
	
	// at this point neither operand is zero
	// prepare the quotient
	LargeInt q(u.len - v.len + 1, 0);
	if (u.sign == v.sign)
		q.sign = 1;
	else
		q.sign = -1;

	// the remainder should be same size as divisor
	delete [] (r->adr);
	r->adr = new int[v.len];
	assert(r->adr != 0);
	memset(r->adr, 0, v.len * sizeof(int));
	r->len = v.len;
	r->sign = u.sign;

	// carry out the division
	divide(u.adr, v.adr, q.adr, r->adr, v.len, u.len - v.len);
	
	// normalize results if necessary
	if (*q.adr == 0)
		q.normalize();
	if (*r->adr == 0)
		r->normalize();
   return q;
}

/* friend evalFrac() - evaluates the fraction u/v where u and v are
   both of type LargeInt, and copies the result as a string of integers
   with precision prec places beyond the decimal point.  The string is
   located at destination dest, where lim is the number of bytes
   available for the string.
   Return: dest if there is enough space for the string
           else 0
   -------------------------------------------------------------------- */
char* evalFrac(const LargeInt& u, const LargeInt& v, int prec,
						char* dest, int lim) {
	LargeInt q; // quotient
	LargeInt r; // remainder
	char* initDest = dest;
	int blocks, lastDigits;
	int i; // loop index

	// abort if divisor is 0
	if (v.sign == 0) {
		fprintf(stderr, "\n\aevalFrac() : division by zero\n");
		exit(EXIT_FAILURE);
	}
	
	// check for special case of u == 0
	if (u.sign == 0)
		if (lim > prec + 3) {
			*dest++ = '0';
			*dest++ = '.';
			memset(dest, '0', prec);
			*(dest + prec) = '\0'; // terminate the string
			return initDest;
		}
		else
			return 0; // lim too small

	// divide, save remainder and process quotient
	// this part comes in front of the decimal point
	*dest = '\0';
	q = divRem(u, v, &r);
	if (q.sign != 0  &&  q.binToDec(dest, lim) == 0)
		return 0; // lim too small
	if (*dest == '\0') {
		// there are no digits before the decimal point except
		// for 0, but we may have to store a minus sign
		if (lim < 3)
			return 0; // lim too small
		if (u.sign != v.sign) {
				*dest       = '-';
				*(dest + 1) = '0';
				*(dest + 2) = '\0';
		}
		else {
			*dest       = '0';
			*(dest + 1) = '\0';
		}
	}

	// at this point we have processed the integer result
	// (if any) of u/v, and we can generate the prec digits
	// beyond the decimal point
	
	// make sure there is enough room in dest[]
	dest += strlen(dest);
	lim -= dest - initDest;
	if (lim < prec + 2) // room for prec + '.' and '\0'?
		return 0; // lim too small
	
	// There is enough room; transfer decimal point
	*dest++ = '.';

	// process blocks of PackFactor digits at a time
	blocks = prec / PackFactor;
	lastDigits = prec % PackFactor;
	for (i = 0; i < blocks; i++) {
		q = r * tenTo9;
		q = divRem(q, v, &r);
		sprintf(dest, "%09u", *q.adr);
		dest += PackFactor;
	}
	
	// process last digits, if any
	if (lastDigits > 0) {
		q = r;
		for (i = 0; i < lastDigits; i++)
			q *= 10;
		q = q / v;
		char format[10]; // needed for runtime formatting
		sprintf(format, "%%0%1d", lastDigits);
		format[3] = 'u';
		format[4] = '\0';
		sprintf(dest, format, *q.adr);
	}
	return initDest;
}


LargeInt LargeInt::operator-() const { // unary minus
	if (sign == 0)
		return zero;
	
	// at this point the operand is not 0
	LargeInt result = *this;
	result.sign = (sign > 0)? -1 : 1;
	return result;
}

///////////////////////////////////////////////////////////////////////
// LargeInt functions

void LargeInt::normalize() {
	int i, *newAdr;
	
	if (*adr == 0  &&  len == 1) { // item is zero!
		sign = 0;
		return;
	}
	if (*adr != 0  ||  len == 1) // already normalized
		return;
	
	i = 1;
	while (adr[i] == 0  &&  i < len)
		i++;
	if (i == len) { // object is zero
		*this = 0;
		return;
	}
	newAdr = new int[len - i];
	assert(newAdr != 0);
	memcpy(newAdr, adr + i, (len - i)*sizeof(int));
	delete [] adr;
	adr = newAdr;
	len = len - i;
}

/* initialize LargeInt to a power of two
   ------------------------------------- */
void LargeInt::powerTwo(int power) {
	delete [] adr;
	// init sign, len and adr
	sign = 1;
	len = 1 + power/(sizeof(int) * 8);
	adr = new int[len];
	assert(adr != 0);
	// zero all, and set the proper bit of adr[0]
	memset(adr, 0, len * sizeof(int));
	*adr = 1 << power % (sizeof(int) * 8);
}

/* display sign and contents of each radix-2^32 integer
   ---------------------------------------------------- */
void LargeInt::lintDump() const {
	printf("sign = %d  ", sign);
	for (int i = 0; i < len; i++) 
		printf("%08X ", adr[i]);
	putchar('\n');
}

void swap9(char* s1, char* s2);
/* Convert multiple-precision radix-2^32 integer to decimal integer
   string and store at dest.  lim is the amount of storage available
   at dest.  Returns dest if successful, else 0.  Method of 
   conversion is as follows: divide repeatedly by 10^PackFactor,
   and then use single-precision operations to generate blocks of
   PackFactor decimal digits for each divide.  Reverse string
   (in blocks of PackFactor characters) when finished.
   ----------------------------------------------------------------- */
char* LargeInt::binToDec(char* dest, int lim) const {
	LargeInt copy = *this;   // convert this copy
	int* copyAdr = copy.adr; // this pointer will be changed
	int copyLen	 = copy.len; // this length will be changed
	int binDec; // holds one 10^PackFactor binary digit
	char* initDest = dest;
	int blkCnt, i, j, leadZeros;
	assert(lim > 0);
	
	// special case of 0
	if (sign == 0) {
		assert(len == 1  &&  *adr == 0);
		strcpy(dest, "0");
		return initDest;
	}
	
	// convert to string in blocks of PackFactor
	blkCnt = 0;
	while (*copyAdr != 0  ||  copyLen > 1) {
		binDec = divrem(copyAdr, intTenTo9, copyLen);
		if (copyLen > 1  &&  *copyAdr == 0) {
			copyAdr++;
			copyLen--;
		}
		if (lim - (PackFactor + 1) > 0) { // 1 extra for null byte
			sprintf(dest, "%09u", binDec);
			lim  -= PackFactor;
			dest += PackFactor;
		}
		else
			return 0; // not enough room for output string
		blkCnt++;
	} // on exit, dest points at null byte
	lim--; // account for the null byte
	
	// swap the output string; blkCnt groups of PackFactor
	i = 0; // subscript of beginning of 1st block
	j = (blkCnt - 1) * PackFactor; // subscript of beginning of last block
	blkCnt &= -2; // we only need an even number of swaps!
	while (blkCnt > 1) {
		swap9(initDest + i, initDest + j);
		blkCnt -= 2;
		i += PackFactor;
		j -= PackFactor;
	}
	
	// find out how many leading zeros
	// it is safe to assume that number is not zero!
	for (i = 0; initDest[i] == '0'; i++) {}
	leadZeros = i;
	if (sign == 1  &&  leadZeros > 0) { // get rid of them!
		for (i = 0; initDest[i] = initDest[i + leadZeros]; i++) {}
	}
	else if (sign == -1  && leadZeros == 0) { // make room for sign
		if (lim <= 0)
			return 0; // no room for sign!
 		for (i = strlen(initDest); i >= 0; i--)
			initDest[i + 1] = initDest[i];
		*initDest = '-';
	}
	else if (sign == -1  &&  leadZeros > 1) {
		// get rid of (leadZeros - 1) leading zeros
		for (i = 1; initDest[i] = initDest[i + leadZeros - 1]; i++) {}
		*initDest = '-';
	}
	return initDest;	
}

inline void swap9(char* s1, char* s2) {
	char hold;

	for (int i = 0; i < PackFactor; i++) {
		hold  = s1[i];
		s1[i] = s2[i];
		s2[i] = hold;
	}
}
		
const char* get9(char* dest, const char* src, int qty);
/* Convert multiple-precision decimal integer string from radix-10
   to binary.  Convert blocks of PackFactor digits at a time as follows:
   Let (um ... u1u0) be the radix-10^PackFactor number.  Use radix-2^32
   arithmetic to evaluate the polynomial umb^m + ... + u1b + u0 by
   using Horner's method ((...(umb + um-1)b + ... u1)b + u0
   Assume that sting str is of the form [sign]nnnmmmnnnmmmnnnmmm\0
   where the sign is optional.  There is no error checking of str.
   See section 4.4 of Knuth, volume 2
   -------------------------------------------------------------------- */
void LargeInt::decToBin(const char* str) {
	char buf[PackFactor + 1]; // PackFactor digits plus null byte
	const char *last;
	int numblocks, numchars, strSign;
	// initial configuration
	if (len != 1) {
		len = 1;
		delete [] adr;
		adr = new int[1];
		assert(adr != 0);
	}
	*adr = 0;
	if (*str == '+') {
		strSign = 1;
		str++;
	}
	else if (*str == '-') {
		strSign = -1;
		str++;
	}
	else
		strSign = 1; // default to +
	// get rid of leading 0s, if any
	while (*str == '0')
		str++;
	// test for case of "zero" string!
	if (*str == '\0') {
		sign = 0;
		return;
	}
	sign = 1; // correct sign will be applied just prior to return
	// find out how many blocks will be transferred
	numblocks = numchars = strlen(str);
	if (numblocks % PackFactor != 0) 
		numblocks = numblocks / PackFactor + 1;
	else
		numblocks /= PackFactor;
	// carry out the intial coversion
	last = get9(buf, str, 1 + (numchars - 1) % PackFactor);
	*this = (unsigned)strtoul(buf, 0, 10);
	// carry out the rest of the conversion
	while (--numblocks > 0) {
		last++;
		last = get9(buf,last, PackFactor);
		*this = (*this) * tenTo9;
		*this = (*this) + (unsigned)strtoul(buf, 0, 10);
	}
	sign = strSign;
}


/* Copy	qty characters and store as string
   Return adr of last source character transferred
   ----------------------------------------------- */
const char* get9(char* dest, const char* src, int qty) {
	assert (*src != '\0');
	for (int i = 0; i < qty  &&  (dest[i] = src[i]); i++)
		{ };
	dest[i] = '\0';
	return src + i - 1;
}
