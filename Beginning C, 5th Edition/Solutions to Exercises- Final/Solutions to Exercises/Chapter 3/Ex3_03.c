// Exercise 3.3 Calculate a discounted price

// I interpreted this exercise as implying that the 10% applies to items 31 to 50
// and the 15% applies to items in excess of 50.
// That is, you don't get 15% discount on the whole price when you order 51 items.

// There is more than one way of doing this so different is not necessarily wrong.

#include <stdio.h>

int main(void)
{
    const int level1 = 30;               // Quantity over this level are at discount1
    const int level2 = 50;               // Quantity over this level are at discount2
    const double discount1 = 0.10;       // 10% discount
    const double discount2 = 0.15;       // 15% discount
    const double unit_price = 5.0;       // Basic unit price
    int quantity = 0;
    int qty_full_price = 0;              // 0 to 30 at full price
    int qty_level1 = 0;                  // 31 to 50 at level1 price
    int qty_level2 = 0;                  // Over 50 at level2 price
    printf("Enter the quantity that you require: ");
    scanf("%d", &quantity);

    if (quantity > 50) {                  // Quantity over 50
        qty_full_price = level1;
        qty_level1 = level2 - level1;
        qty_level2 = quantity - level2;
    } else if (quantity > 30) {           // Quantity is from 30 to 50
        qty_full_price = level1;
        qty_level1 = quantity - level1;
    } else
        qty_full_price = quantity;

    printf("The total price for %d items is $%.2lf\n", quantity,
           unit_price * (qty_full_price + (1.0 - discount1)*qty_level1 + (1.0 - discount2)*qty_level2));
    return 0;
}
