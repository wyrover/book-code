// Program 14.3 Working with complex numbers
#define __STDC_WANT_LIB_EXT1__ 1
#include <complex.h>
#include <stdio.h>

int main(void)
{
#ifdef __STDC_NO_COMPLEX__
    printf_s("Complex numbers are not supported.\n");
    exit(1);
#else
    printf_s("Complex numbers are supported.\n");
#endif
    double complex cx = 1.0 + 3.0 * I;
    double complex  cy = 1.0 - 4.0 * I;
    printf_s("Working with complex numbers:\n");
    printf_s("Starting values: cx = %.2f%+.2fi  cy = %.2f%+.2fi\n",
             creal(cx), cimag(cx), creal(cy), cimag(cy));
    double complex  sum = cx + cy;
    printf_s("\nThe sum cx + cy = %.2f%+.2fi\n",
             creal(sum), cimag(sum));
    double complex  difference = cx - cy;
    printf_s("The difference cx - cy = %.2f%+.2fi\n", creal(difference), cimag(difference));
    double complex product = cx * cy;
    printf_s("The product    cx * cy = %.2f%+.2fi\n",
             creal(product), cimag(product));
    double complex quotient = cx / cy;
    printf_s("The quotient   cx / cy = %.2f%+.2fi\n",
             creal(quotient), cimag(quotient));
    double complex conjugate = conj(cx);
    printf_s("\nThe conjugate of cx =  %.2f%+.2fi\n",
             creal(conjugate) , cimag(conjugate));
    return 0;
}

