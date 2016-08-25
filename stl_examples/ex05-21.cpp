// Illustrating the generic unique algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>
#include <iostream.h>

int main()
{
  const int N = 11;
  int array1[N] = {1, 2, 0, 3, 3, 0, 7, 7, 7, 0, 8};
  vector<int> vector1;
  for (int i = 0; i < N; ++i)
    vector1.push_back(array1[i]);

  // Eliminate consecutive duplicates from vector1:
  vector<int>::iterator new_end;
  new_end = unique(vector1.begin(), vector1.end());

  // The size of vector1 remains the same;
  assert(vector1.size() == N);

  // The nonconsecutive duplicate elements are left in 
  // [vector1.begin(), new_end).  Erase the rest:
  vector1.erase(new_end, vector1.end());

  // Put the resulting vector1 contents on the 
  // standard output stream:
  copy(vector1.begin(), vector1.end(), 
       ostream_iterator<int>(cout, " "));
  cout << endl;

}
