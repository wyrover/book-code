====================== Listing 1 ======================
/* fact1.cpp
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
   for (int i = 2; i <= choice; i++)
      result *= i;
   if (result.binToDec(buf, bufSize) != NULL) {
      printf("\n%d! = \n%s\n", choice, buf);
      printf("\nwhich has %d digits\n", strlen(buf));
   }
   else
      printf("Output string buffer too small\n");

   return 0;
}
