// Program 10.3 Floating-Point Input
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    float fp1 = 0.0f;
    float fp2 = 0.0f;
    float fp3 = 0.0f;
    int value_count = 0;
    printf("Enter: 3.14.314E1.0314e+02\n");
    printf("Input:\n");
//  value_count = scanf_s("%f %f %f", &fp1, &fp2, &fp3);
    value_count = scanf_s("%e %g %a", &fp1, &fp2, &fp3);           // Alternative
    printf("\nOutput:\n");
    printf("Number of values read = %d\n", value_count);
    printf("fp1 = %f  fp2 = %f  fp3 = %f\n", fp1, fp2, fp3);
    return 0;
}
