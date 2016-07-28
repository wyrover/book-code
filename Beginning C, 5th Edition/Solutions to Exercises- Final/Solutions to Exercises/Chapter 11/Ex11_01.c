// Exercise 11.1 Using a structure representing a length.
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <ctype.h>

#define INCHES_PER_FOOT 12
#define FEET_PER_YARD    3

typedef struct Length {
    unsigned int yards;
    unsigned int feet;
    unsigned int inches;
} Length;

Length add(const Length first, const Length second);
void show(const Length length);

int main(void)
{
    char answer = 'n';
    Length length = {0};
    Length total = {0};

    do {
        printf_s("Enter a length in yards, feet, and inches: ");
        scanf_s(" %u %u %u", &length.yards, &length.feet, &length.inches);
        total = add(total, length);
        printf_s("Do you want to enter another(y or n)?: ");
        scanf_s(" %c", &answer, sizeof(answer));
        fflush(stdin);
    } while (tolower(answer) == 'y');

    printf_s("The total of all the lengths is: ");
    show(total);
    printf_s("\n");
    return 0;
}

struct Length add(const Length first, const Length second)
{
    unsigned long inches = 0;
    Length sum;
    inches = first.inches + second.inches +
             INCHES_PER_FOOT * (first.feet + second.feet + FEET_PER_YARD * (first.yards + second.yards));
    sum.inches = inches % INCHES_PER_FOOT;
    sum.feet = inches / INCHES_PER_FOOT;
    sum.yards = sum.feet / FEET_PER_YARD;
    sum.feet %= FEET_PER_YARD;
    return sum;
}

void show(const Length length)
{
    printf_s("%u yards %u feet %u inches", length.yards, length.feet, length.inches);
}
