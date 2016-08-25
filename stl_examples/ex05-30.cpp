// Illustrating the generic permutation algorithms.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  // Initialize a vector of integers: 
  vector<int> vecInt(3);
  for(int i = 0; i < 3; i++) vecInt[i] = i;

  // In lexicographical order the permutations of 0 1 2 are
  // 0 1 2, 0 2 1, 1 0 2, 1 2 0, 2 0 1, 2 1 0. 
  // Show that from 0 1 2 next_permutation produces 0 2 1:
  next_permutation(vecInt.begin(), vecInt.end());
  assert(vecInt[0] == 0);
  assert(vecInt[1] == 2);
  assert(vecInt[2] == 1);

  // Show that from 0 2 1 prev_permutation() produces 0 1 2:
  prev_permutation(vecInt.begin(), vecInt.end());
  assert(vecInt[0] == 0);
  assert(vecInt[1] == 1);
  assert(vecInt[2] == 2);
}
