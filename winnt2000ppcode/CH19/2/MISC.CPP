/* misc.cpp
   measure cpu seconds:
   timer(0) starts timer
   timer(1) stops timer and displays result
   in the form:  nn.n seconds
   Copyright (C) 1994 John K. Gotwals
   ---------------------------------------- */
#include <time.h>
#include <stdio.h>

void timer(int f) {
    static clock_t time1;
    unsigned timediff;

#pragma comment(exestr, "Copyright (C) 1994 John K. Gotwals")

    if (f == 0)
        time1 = clock();
    else {
        timediff = (unsigned)(clock() - time1) * 1000 / CLOCKS_PER_SEC;
        timediff += 50;
        timediff /= 100;
        printf("%u", timediff / 10);
        printf(".%u seconds", timediff % 10);
    }
    return ;
}

/* Pause until enter is pressed
   ---------------------------- */
void pause() {
	printf("\nPress enter to continue - ");
	while (getchar() != '\n');
}

