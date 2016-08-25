// Illustrating the generic heap operations.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  // Initialize a vector of integers: 
  vector<int> vecInt(5);
  for (int i = 0; i < 5; i++)
    vecInt[i] = i;

  random_shuffle(vecInt.begin(), vecInt.end());

  // Sort the vector using push_heap and pop_heap:
  for (i = 2; i < 5; i++)
    push_heap(vecInt.begin(), vecInt.begin() + i);
  
  for (i = 5; i >= 2; i--)
    pop_heap(vecInt.begin(), vecInt.begin() + i);
  
  // Verify that the array is sorted:
  for (i = 0; i < 5; i++)   
    assert(vecInt[i] == i);

  // Shuffle the elements again:
  random_shuffle(vecInt.begin(), vecInt.end());

  // Sort the vector using make_heap and sort_heap:
  make_heap(vecInt.begin(), vecInt.end());
  sort_heap(vecInt.begin(), vecInt.end());

  // Verify that the array is sorted:
  for (i = 0; i < 5; i++) assert(vecInt[i] == i);
}
