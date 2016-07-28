// Ex2_03.cpp
// Understanding how capacity is increased in a vector container
#include <iostream>                             // For standard streams
#include <vector>                               // For vector container

int main()
{
    std::vector <size_t> sizes;                    // Record numbers of elements
    std::vector <size_t> capacities;               // and corresponding capacities
    size_t el_incr {10};                           // Increment to initial element count
    size_t incr_count {4 * el_incr};               // Number of increments to element count

    for (size_t n_elements {}; n_elements < incr_count; n_elements += el_incr) {
        std::vector<int> values(n_elements);
        std::cout << "\nAppending to a vector with " << n_elements << " initial elements:\n";
        sizes.push_back(values.size());
        size_t space {values.capacity()};
        capacities.push_back(space);
        // Append elements to obtain capacity increases
        size_t count {};                             // Counts capacity increases
        size_t n_increases {10};

        while (count < n_increases) {
            values.push_back(22);                      // Append a new element

            if (space < values.capacity()) {           // Capacity increased...
                // ...so record size and capacity
                space = values.capacity();
                capacities.push_back(space);
                sizes.push_back(values.size());
                ++count;
            }
        }

        // Show sizes & capacities when increments occur
        std::cout << "Size/Capacity: ";

        for (size_t i {}; i < sizes.size(); ++i)
            std::cout << sizes.at(i) << "/" << capacities.at(i) << "  ";

        std::cout << std::endl;

        sizes.clear();                               // Remove all elements

        capacities.clear();                          // Remove all elements
    }
}