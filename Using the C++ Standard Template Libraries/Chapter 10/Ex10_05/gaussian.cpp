// Gaussian.cpp
// Functions to implement Gaussian elimination
#include <valarray>                              // For valarray, slice, abs()
#include <vector>                                // For vector container
#include <iterator>                              // For ostream iterator
#include <algorithm>                             // For copy_n() 
#include <utility>                               // For swap() 
#include <iostream>                              // For standard streams
#include <iomanip>                               // For stream manipulators
using std::valarray;
using std::slice;

// Read the data for n equations in n unknowns
valarray<double> get_data(size_t n)
{
    valarray<double> equations(n * (n + 1));  // n rows of n+1 elements
    std::cout << "Enter " << n + 1
              << " values for each of " << n << " equations.\n"
              << "(i.e. including coefficients that are zero and the rhs:\n";

    for (auto& coeff : equations) std::cin >> coeff;

    return equations;
}

// Selects the best pivot in row n (rows indexed from 0)
void set_pivot(const valarray<double>& equations, std::vector<slice>& row_slices, size_t n)
{
    size_t n_rows {row_slices.size()};             // Number of rows
    size_t row_len {n_rows + 1};                   // Row length = number of columns
    // Create an object containing the elements in column n, starting row n
    valarray<double> column {equations[slice {n * row_len + n, n_rows - n, row_len}]};
    column = std::abs(column);                     // Absolute values
    size_t max_index {};                            // Index to best pivot in column

    for (size_t i {1}; i < column.size(); ++i) {    // Find index for max value
        if (column[max_index] < column[i]) max_index = i;
    }

    if (max_index > 0) {
        // Pivot is not the 1st in column - so swap rows to make it so
        std::swap(row_slices[n], row_slices[n + max_index]);
    } else if (!column[0]) {                       // Check for zero pivot
        // When pivot is 0, matrix is singular
        std::cerr << "No solution. Ending program." << std::endl;
        std::exit(1);
    }
}

// Reduce the equations matrix to row echelon form
// The equations object stores the matrix for n equations in n unknowns
// The elements in the row_slices container define the rows in equations
void reduce_matrix(valarray<double>& equations, std::vector<slice>& row_slices)
{
    size_t n_rows {row_slices.size()};             // Number of rows
    size_t row_len {n_rows + 1};                   // Row length

    for (size_t row {}; row < n_rows - 1; ++row) { // From 1st row to second-to-last
        set_pivot(equations, row_slices, row);       // Find best pivot
        // Create an object containing element values for pivot row
        valarray<double> pivot_row {equations[row_slices[row]]};
        auto pivot = pivot_row[row];
        pivot_row /= pivot;                          // Divide pivot row by pivot

        // For each of the rows following the current row,
        // subtract the pivot row multiplied by the row element in the pivot column
        for (size_t next_row {row + 1}; next_row < n_rows; ++next_row) {
            equations[row_slices[next_row]] -= equations[row_slices[next_row].start() + row] * pivot_row;
        }
    }
}

// Perform back substitution and return the solution
valarray<double> back_substitution(valarray<double>& equations, const std::vector<slice>& row_slices)
{
    size_t n_rows{row_slices.size()};
    size_t row_len {n_rows + 1};
    // Divide the last row by the second to last element
    // Multiply the last row by the second to last element in each row and subtract it from the row.
    // Repeat for all the other rows
    valarray<double> results(n_rows);           // Stores the solution

    for (int row {static_cast<int>(n_rows - 1)}; row >= 0; --row) {
        equations[row_slices[row]] /= valarray<double>(equations[row_slices[row].start() + row], row_len);
        valarray<double> last_row {equations[row_slices[row]]};
        results[row] = last_row[n_rows];          // Store value for x[row]

        for (int i {}; i < row; ++i) {
            equations[row_slices[i]] -= equations[row_slices[i].start() + row] * last_row;
        }
    }

    return results;
}
