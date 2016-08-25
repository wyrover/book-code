// Illustrating the generic min_element and max_element algorithms.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  // Initialize a vector of integers: 
  vector<int> vecInt(5);
  for(int i = 0; i < 5; i++) 
    vecInt[i] = i;
  random_shuffle(vecInt.begin(), vecInt.end());

  // Find the max_element in the container:
  vector<int>::iterator vecIter;

  vecIter = max_element(vecInt.begin(), vecInt.end());
  assert(*vecIter == 4);

  // Find the min_element in the container:
  vecIter = min_element(vecInt.begin(), vecInt.end());
  assert(*vecIter == 0);
}
