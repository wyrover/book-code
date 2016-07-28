// Ex10_03.cpp
// Using the Gaussian Elimination method to solve a set of liear equations
#include <valarray>                              // For valarray, slice, abs()
#include <vector>                                // For vector container
#include <iterator>                              // For ostream iterator
#include <algorithm>                             // For generate_n() 
#include <utility>                               // For swap() 
#include <iostream>                              // For standard streams
#include <iomanip>                               // For stream manipulators
#include <string>                                // For string type
using std::string;
using std::valarray;
using std::slice;

// Function prototypes
valarray<double> get_data(size_t n);
void reduce_matrix(valarray<double>& equations, std::vector<slice>& row_slices);
valarray<double> back_substitution(valarray<double>& equations, const std::vector<slice>& row_slices);

int main()
{
    size_t n_rows {};
    std::cout << "Enter the number of variables: ";
    std::cin >> n_rows;
    auto equations = get_data(n_rows);
    // Generate slice objects for rows in row order
    std::vector<slice> row_slices;                           // Objects define rows in sequence
    size_t row_len {n_rows + 1};
    std::generate_n(std::back_inserter(row_slices), n_rows,
    [row_len]() {
        static size_t index {};
        return slice {row_len*index++, row_len, 1};
    });
    reduce_matrix(equations, row_slices);                    // Reduce to row echelon form
    auto solution = back_substitution(equations, row_slices);
    // Output the solution
    size_t count {}, perline {8};
    std::cout << "\nSolution:\n";
    string x{"x"};

    for (const auto& v : solution) {
        std::cout << std::setw(3) << x + std::to_string(count + 1) << " = "
                  << std::fixed << std::setprecision(2) << std::setw(10)
                  << v;

        if (++count % perline) std::cout << '\n';
    }

    std::cout << std::endl;
}