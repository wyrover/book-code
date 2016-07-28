// Program 10.5 Reading characters with scanf_s()
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#define MAX_TOWN 10
int main(void)
{
    char initial = ' ';
    char name[80] = { ' ' };
    char age[4] = { '0' };
    printf("Enter your first initial: ");
    scanf_s("%c", &initial, sizeof(initial));
    printf("Enter your first name: ");
    scanf_s("%s", name, sizeof(name));

    if (initial  != name[0])
        printf("%s,you got your initial wrong.\n", name);
    else
        printf("Hi, %s. Your initial is correct. Well done!\n", name);

    printf("Enter your full name and your age separated by a comma:\n");
    scanf_s("%[^,] , %[0123456789]", name, sizeof(name), age, sizeof(age));
    printf("\nYour name is %s and you are %s years old.\n", name, age);
    return 0;
}
