/* fact.cpp
   demo factorial program
   ---------------------- */
#include <stdio.h>
#include <string.h>
#include "largeint.h"
#include "misc.h"

int main() {
	const int bufSize = 50000; // max decimal digits
	char buf[bufSize];
	int choice;
	LargeInt result = 1;

	printf("Calculate factorial of: ");
	scanf("%d", &choice);
	while(getchar() != '\n'); // get rid of newline
	timer(0);
	for (int i = 2; i <= choice; i++)
		result *= i;
	printf("The time to calculate %d! is ", choice), timer(1);
	timer(0);
	if (result.binToDec(buf, bufSize) != NULL) {
		printf("\nThe conversion time is "), timer(1);
		pause();
		printf("\n%d! = \n%s\n", choice, buf);
		printf("\nwhich has %d digits\n", strlen(buf));
	}
	else
		printf("Output string buffer too small\n");
	
	pause();
	return 0;
}
