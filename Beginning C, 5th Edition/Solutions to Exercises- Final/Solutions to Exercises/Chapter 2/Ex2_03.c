// Exercise 2.3  Calculating volume price of alternative products

// The only problem here is to devise a way to determine the price
// for the product type. Here I used the product type value to do this.
#include <stdio.h>

int main(void)
{
    double total_price = 0.0;                // Total price
    int type = 0;                            // Product type
    int quantity = 0;                        // Quantity ordered
    const double type1_price = 3.50;
    const double type2_price = 5.50;
    // Get the product type
    printf("Enter the type (1 or 2): ");
    scanf("%d", &type);
    // Get the order quantity
    printf("Enter the quantity: ");
    scanf("%d", &quantity);
    // Calculate the total price
    total_price = quantity * (type1_price + (type - 1) * (type2_price - type1_price));
    // Output the area
    printf("The price for %d of type %d is $%.2f\n", quantity, type, total_price);
    return 0;
}
