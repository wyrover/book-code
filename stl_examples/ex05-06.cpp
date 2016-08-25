// Illustrating the generic count algorithm.
#include <function.h>
#include <algo.h>
#include <assert.h>

int main()
{
  int a[] = {0, 0, 0, 1, 1, 1, 2, 2, 2};
  int final_count = 0;

  // Count the number of values in the array a
  // that are equal to 1:
  count(&a[0], &a[9], 1, final_count);

  assert(final_count == 3);
  
  // Determine the number of array elements that are not 
  // equal to 1:
  final_count = 0;
  count_if(a, a + 9, bind2nd(not_equal_to<int>(), 1),
           final_count);

  assert(final_count == 6);
  // There are 6 elements not equal to 1.
}
