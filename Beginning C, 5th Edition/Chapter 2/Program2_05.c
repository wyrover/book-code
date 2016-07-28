// Program 2.5 Calculations with cookies
#include <stdio.h>

int main(void)
{
    int cookies = 5;
    int cookie_calories = 125;                // Calories per cookie
    int total_eaten = 0;                      // Total cookies eaten
    int eaten = 2;                            // Number to be eaten
    cookies = cookies - eaten;                // Subtract number eaten from cookies
    total_eaten = total_eaten + eaten;
    printf("\nI have eaten %d cookies.  There are %d cookies left",
           eaten, cookies);
    eaten = 3;                                // New value for cookies eaten
    cookies = cookies - eaten;                // Subtract number eaten from cookies
    total_eaten = total_eaten + eaten;
    printf("\nI have eaten %d more.  Now there are %d cookies left\n", eaten, cookies);
    printf("\nTotal energy consumed is %d calories.\n", total_eaten * cookie_calories);
    return 0;
}
