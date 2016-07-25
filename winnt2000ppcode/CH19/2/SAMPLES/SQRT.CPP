/* sqrt1.cpp
   generate sqrt(2) to 40 places
   ----------------------------- */
#include <stdio.h>
#include <string.h>
#include "largeint.h"
#include "misc.h"

int main() {
	const int size = 100;
	char buf[size];
	LargeInt n1 = 2, result;
	int len;
	
	// multiply n1 by 10^82
	for (int i = 0; i < 82; i++)
		n1 *= 10;
	result = sqrt(n1);
	result.binToDec(buf, size);
	len = strlen(buf);
	printf("sqrt(2) = 1.%s\n", buf + 1);
	n1 = result * result;
	n1.binToDec(buf, size);
	buf[len] = '\0'; // terminate the string
	printf("square  = 1.%s\n", buf + 1);

	pause();
	return 0;
}

/* Program output
sqrt(2) = 1.41421356237309504880168872420969807856967
square  = 1.99999999999999999999999999999999999999999
*/
