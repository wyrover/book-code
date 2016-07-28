// Listing 2-1. Reading Test
/// Read the program and determine what the program does.
 
#include <iostream>
#include <limits>
 
int main()
{
    int min{std::numeric_limits<int>::max()};
    int max{std::numeric_limits<int>::min()};
    bool any{false};
    int x;
    while (std::cin >> x)
    {
        any = true;
        if (x < min)
            min = x;
        if (x > max)
            max = x;
    }

    if (any)
        std::cout << "min = " << min << "\nmax = " << max << '\n';
}
