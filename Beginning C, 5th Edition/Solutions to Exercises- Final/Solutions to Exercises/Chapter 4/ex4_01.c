//Exercise 4.1 Generate a multiplication table */
#include <stdio.h>

int main(void)
{
    int table_size = 0;                       // Table size        */
    printf("Enter the table size (from 2 to 12): ");
    scanf("%d", &table_size);

    if (table_size > 12) {
        printf("Table size must not exceed 12 - setting to 12\n");
        table_size = 12;
    } else if (table_size < 2) {
        printf("Table size must be at least 2 - setting to 2\n");
        table_size = 2;
    }

    for (int row = 0 ; row <= table_size ; ++row) {
        printf("\n");                      // Start new row

        for (int col = 0 ; col <= table_size ; ++col) {
            if (row == 0) {                  // 1st row?
                // Yes - output column headings
                if (col == 0)                  // 1st column?
                    printf("    ");              // Yes - no heading
                else
                    printf("|%4d", col);         //No - output heading
            } else {
                // Not 1st row - output rows
                if (col == 0)                  // 1st column?
                    printf("%4d", row);          // Yes - output row label
                else
                    printf("|%4d", row * col);   // No - output table entry
            }
        }

        if (row == 0) {                    // If we just completed 1st row
            // output separator dashes
            printf("\n");

            for (int col = 0 ; col <= table_size ; ++col)
                printf("_____");
        }
    }

    printf("\n");
    return 0;
}

