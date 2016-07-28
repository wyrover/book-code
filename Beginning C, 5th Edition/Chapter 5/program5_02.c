// Program 5.2 Averaging ten grades - storing values the hard way
#include <stdio.h>

int main(void)
{
    int grade0 = 0, grade1 = 0, grade2 = 0, grade3 = 0, grade4 = 0;
    int grade5 = 0, grade6 = 0, grade7 = 0, grade8 = 0, grade9 = 0;
    long sum = 0L;          // Sum of the grades
    float average = 0.0f;   // Average of the grades
    // Read the ten grades to be averaged
    printf("Enter the first five grades,\n");
    printf("use a space or press Enter between each number.\n");
    scanf("%d%d%d%d%d", & grade0, & grade1, & grade2, & grade3, & grade4);
    printf("Enter the last five numbers in the same manner.\n");
    scanf("%d%d%d%d%d", & grade5, & grade6, & grade7, & grade8, & grade9);
    // Now we have the ten grades, we can calculate the average
    sum = grade0 + grade1 + grade2 + grade3 + grade4 +
          grade5 + grade6 + grade7 + grade8 + grade9;
    average = (float)sum / 10.0f;
    printf("\nAverage of the ten grades entered is: %f\n", average);
    return 0;
}
