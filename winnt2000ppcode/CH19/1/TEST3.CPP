====================== Listing 8 ======================
/* test3.cpp
   Lucas-Lehmer test for primality of 2^p - 1
   If p > 2 is a prime, then 2^p - 1 is prime if
   and only if L[p-2] = 0, where the sequence L[i]
   is defined as follows: L[0] = 4,
   L[i+1] = (L[i]^2 - 2) modulo (2^p - 1)
   ----------------------------------------------- */
#include <stdio.h>
#include "largeint.h"

void pause();
void timer(int f);

int main() {
   int p, i;
   LargeInt L, mod;

   printf("Enter a prime number: ");
   scanf("%d", &p);
   timer(0); // start timer
   mod.powerTwo(p);
   mod = mod - 1; // mod = 2^p - 1
   L = 4;
   for (i = 2; i <= p - 1; i++) {
      if (i % 100 == 0)
         printf("%4d\r", i);
      L = (L * L - 2) % mod;
   }
   printf("\n2^%d - 1 is ", p);
   if (L == zero)
      printf("prime\n");
   else
      printf("not prime\n");
   printf("and the calculation took "), timer(1);

   return 0;
}
