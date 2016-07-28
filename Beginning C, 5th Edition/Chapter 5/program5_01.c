// Program 5.1 Averaging ten grades without storing them
#include <stdio.h>

int main(void)
{
    int grade = 0;                       // Stores a grade
    unsigned int count = 10;             // Number of values to be read
    long sum = 0L;                       // Sum of the grades
    float average = 0.0f;                // Average of the grades

    // Read the ten grades to be averaged
    for (unsigned int i = 0 ; i < count ; ++i) {
        printf("Enter a grade: ");
        scanf("%d", & grade);              // Read a grade
        sum += grade;                      // Add it to sum
    }

    average = (float)sum / count;        // Calculate the average
    printf("\nAverage of the ten grades entered is: %f\n", average);
    return 0;
}
