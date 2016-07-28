// Listing 8-7. Printing a Multiplication Table 
#include <iomanip>
#include <iostream>

int main()
{
  using namespace std;

  int const low{1};        ///< Minimum value for the table
  int const high{10};      ///< Maximum value for the table
  int const colwidth{4};   ///< Fixed width for all columns

  // All numbers must be right-aligned.
  cout << right;

  // First print the header.
  cout << setw(colwidth) << '*'
       << '|';
  for (int i{low}; i <= high; i = i + 1)
    cout << setw(colwidth) << i;
  cout << '\n';

  // Print the table rule by using the fill character.
  cout << setfill('-')
       << setw(colwidth) << ""                    // one column's worth of "-"
       << '+'                                     // the vert. & horz. intersection
       << setw((high-low+1) * colwidth) << ""     // the rest of the line
       << '\n';

  // Reset the fill character.
  cout << setfill(' ');

  // For each row...
  for (int row{low}; row <= high; row = row + 1)
  {
    cout << setw(colwidth) << row << '|';
    // Print all the columns.
    for (int col{low}; col <= high; col = col + 1)
      cout << setw(colwidth) << row * col;
    cout << '\n';
  }
}
