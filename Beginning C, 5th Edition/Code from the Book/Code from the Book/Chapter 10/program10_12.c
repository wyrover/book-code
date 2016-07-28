/* Program 10.11 Printing on a printer - where else? */
#include <stdio.h>
int main(void)
{
    fprintf(stdprn, "The barber shaves all those who do not"
            " shave themselves.");
    fprintf(stdprn, "\n\rQuestion: Who shaves the barber?\n\r");
    fprintf(stdprn, "\n\rAnswer: She doesn't need to shave.\f");
    return 0;
}

