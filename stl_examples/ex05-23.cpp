// Illustrating the generic nth_element algorithm.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  vector<int> v(7);

  v[0] = 25; v[1] = 7; v[2] = 9;
  v[3] = 2; v[4] = 0; v[5] = 5; v[6] = 21;

  const int N = 4;

  // Use nth_element to place the Nth smallest 
  // element in v in the Nth position, v.begin() + N:

  nth_element(v.begin(), v.begin() + N, v.end());
  
  // Check that the element at v.begin() + N, v[N], is
  // greater than or equal to each of the preceding elements:
  for (int i = 0; i != N; ++i)
    assert(v[N] >= v[i]);
  
  // Check that the element at v.begin() + N, v[N], is
  // less than or equal to each of the following elements:
  for (i = N + 1; i != 7; ++i)
    assert(v[N] <= v[i]);
}
