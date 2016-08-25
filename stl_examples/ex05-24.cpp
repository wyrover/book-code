// Illustrating the generic binary search algorithms.
#include <algo.h>
#include <vector.h>
#include <assert.h>

int main()
{
  vector<int> v(5);
  bool found;

  // Initialize:
  for(int i = 0; i < 5; i++) v[i] = i; 
 
  // Search for each of the integers 0, 1, 2, 3, 4:
  for (i = 0; i < 5; i++) {
    found = binary_search(v.begin(), v.end(), i);
    assert(found == true);
  }

  // Try searching for a value that's not present:
  found = binary_search (v.begin(), v.end(), 9);
  assert(found == false);

  v[1] = 7; v[2] = 7; v[3] = 7; v[4] = 8;

  // Vector v now contains: 0 7 7 7 8

  typedef vector<int>::iterator iter;
  iter viter;

  // Apply upper_bound, lower_bound and equal_range on v:
 
  viter = lower_bound(v.begin(), v.end(), 7);
  assert(viter == v.begin() + 1 && *viter == 7);

  viter = upper_bound(v.begin(), v.end(), 7);
  assert(viter == v.end() - 1 && *viter == 8);

  pair<iter, iter> pIter = 
      equal_range(v.begin(), v.end(), 7);

  assert(pIter.first == v.begin() + 1);
  assert(pIter.second == v.end() - 1);
}
