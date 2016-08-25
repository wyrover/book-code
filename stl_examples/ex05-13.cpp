// Illustrating the generic partition and stable_partition algorithms.
#include <algo.h>
#include <vector.h>
#include <bstring.h>
#include <iostream.h>

bool above40(int n) { return (n > 40); }

int main()
{
  const int N = 7;
  int array0[N] = {50, 30, 10, 70, 60, 40, 20};
  int array1[N];
  copy(&array0[0], &array0[N], &array1[0]);
  ostream_iterator<int> out(cout, " ");

  cout << "Original sequence:                 ";
  copy(&array1[0], &array1[N], out), cout << endl;

  // Partition array1, putting numbers greater than 40
  // first, followed by those less than or equal to 40:
  int* split = partition(&array1[0], &array1[N], above40);

  cout << "Result of (unstable) partitioning: ";
  copy(&array1[0], split, out), cout << "| ";
  copy(split, &array1[N], out), cout << endl;

  // Restore array1 to array0 contents:
  copy(&array0[0], &array0[N], &array1[0]);

  // Again partition array1, putting numbers greater than 40
  // first, followed by those less than or equal to 40,
  // preserving relative order in each group:
  split = stable_partition(&array1[0], &array1[N], above40);

  cout << "Result of stable partitioning:     ";
  copy(&array1[0], split, out), cout << "| ";
  copy(split, &array1[N], out), cout << endl;
}
