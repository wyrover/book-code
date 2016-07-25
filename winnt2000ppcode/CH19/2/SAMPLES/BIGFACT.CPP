/* bigfact.cpp
   Compute 300000! which has about 1,500,000 digits
   W A R N I N G  - may take 24 hours!
   ------------------------------------------------ */
#include <stdio.h>
#include <string.h>
#include "largeint.h"

void pause();
void timer(int f);

int main() {
	const int size = 2000000;
	static char buf[size];
	const int fact = 300000;
	const char *file = "bigfact.txt";
	int length;
	FILE *stream;
	LargeInt n1;

	printf("Generate %d factorial\n", fact);
	printf("\n- - - W A R N I N G - - -\a\n");
	printf("This program may take a _long_ time to finish!\n\n");
	timer(0); // reset timer
	n1 = 1;
	for (int i = 2; i <= fact; i++) {
		n1 *= i;
		if (i % 100 == 0)
			printf("%5d\r", i);
	}
	printf("Time to calculate %d! is ", fact), timer(1);
	timer(0); // reset timer
	n1.binToDec(buf, size);
	printf("\nTime to convert %d! is ", fact), timer(1);
	length = strlen(buf);
	printf("\n%d! has %d digits\n", fact, length);
	if ((stream	= fopen(file, "w")) == 0) {
		printf("Could not open file %s\n", file);
		pause();
		return 1;
	}
	fprintf(stream, "%d! =\n%s\n",fact, buf);
	fprintf(stream, "which has %d digits\n", length);
	if (fclose(stream)) {
		printf("Could not close file %s\n", file);
		pause();
		return 2;
	}
	pause();
	return 0;
}

/* Pause until enter is pressed
   ---------------------------- */
void pause() {
	printf("\nPress enter to continue - ");
	while (getchar() != '\n');
}

