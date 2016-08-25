// Illustrating the generic transform algorithm.
#include <algo.h>
#include <iostream.h>

int sum(int val1, int val2) { return val2 + val1; }

int main()
{
  int array1[5] = {0, 1, 2, 3, 4};
  int array2[5] = {6, 7, 8, 9, 10};
  ostream_iterator<int> out(cout, " ");

  // Put sums of corresponding array1 and array2 elements 
  // into output stream
  transform(&array1[0], &array1[5], &array2[0], out, sum);

  cout << endl;
}
