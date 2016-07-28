//Exercise 5.5 Calculating average student grades.
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

int main(void)
{
    size_t nclasses = 0;                  // Number classes
    size_t nstudents_max = 0;             // Maximum number of students in a class
    char answer = 'N';
    printf("How many students are in the largest class? :");
    scanf("%zd", &nstudents_max);
    printf("How many classes are there? :");
    scanf("%zd", &nclasses);
    int grades[nclasses][nstudents_max];                     // Stores the grades
    size_t students[nclasses];                               // Stores the number of students in each class

    for (size_t class = 0 ; class < nclasses ; ++class) {
        printf("Enter the grades for students in class %zd.\n", class + 1);
        students[class] = 0;                                   // Student count within a class

        while (true) {
            printf("Enter the integer grade for student %zd: ", students[class] + 1);
            scanf("%d", &grades[class][students[class]]);

            if (++students[class] == nstudents_max) {             // Increment and check student count
                printf("Class %zd complete.", class + 1);
                break;
            }

            printf("Any more students in class %zd (Y or N): ", class + 1);
            scanf(" %c", &answer);

            if (toupper(answer) == 'N')
                break;
        }
    }

    printf("\n");

    for (size_t class = 0 ; class < nclasses ; ++class) {
        int class_total = 0;
        printf("Student grades for class %zd are:\n", class + 1);

        for (size_t student = 0 ; student < students[class] ; ++student) {
            class_total += grades[class][student];

            if ((student + 1) % 6 == 0)
                printf("\n");

            printf("%5d", grades[class][student]);
        }

        printf("\nAverage grade for class %zd is %.2lf\n", class + 1, (double)class_total / students[class]);
    }

    return 0;
}

