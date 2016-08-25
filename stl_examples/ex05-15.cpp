// Illustrating the generic remove algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>
int main()
{
  const int  N = 11;
  int array1[N] = {1, 2, 0, 3, 4, 0, 5, 6, 7, 0, 8};
  vector<int> vector1;
  for (int i = 0; i < N; ++i)
    vector1.push_back(array1[i]);

  // Remove the zeros from vector1:
  vector<int>::iterator new_end;
  new_end = remove(vector1.begin(), vector1.end(), 0);

  // The size of vector1 remains the same:
  assert(vector1.size() == N);

  // The nonzero elements are left in 
  // [vector1.begin(), new_end).  Erase the rest:
  vector1.erase(new_end, vector1.end());

  // Show that 3 elements were removed and the
  // nonzero elements remain, in their original order:
  assert(vector1.size() == N - 3);
  for (i = 0; i < vector1.size(); ++i)
    assert(vector1[i] == i+1);
}
