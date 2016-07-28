// Exercise 3.2 Display a date
#include <stdio.h>

int main(void)
{
    int month = 0;
    int day = 0;
    int year = 0;
    printf("Enter the date as three integer values separated by spaces (month day year): ");
    scanf("%d", &month);
    scanf("%d", &day);
    scanf("%d", &year);

    if (day > 3 && day < 21 || day > 23 && day < 31)
        printf("\n%dth ", day);
    else
        printf("\n%d%s ", day, (day % 10 == 1 ? "st" : (day % 10 == 2 ? "nd" : "rd")));

    switch (month) {
    case 1:
        printf("January ");
        break;

    case 2:
        printf("February ");
        break;

    case 3:
        printf("March ");
        break;

    case 4:
        printf("April ");
        break;

    case 5:
        printf("May");
        break;

    case 6:
        printf("June");
        break;

    case 7:
        printf("July");
        break;

    case 8:
        printf("August");
        break;

    case 9:
        printf("September");
        break;

    case 10:
        printf("October");
        break;

    case 11:
        printf("November");
        break;

    case 12:
        printf("December");
        break;
    }

    printf(" %d\n", year);
    return 0;
}
