// Using the generic sort algorithm with a binary predicate.
#include <algo.h>
#include <function.h>  
#include <assert.h>

int main() {
  int a[1000];
  for (int i = 0; i < 1000; i++) 
    a[i] = i;
  random_shuffle(&a[0], &a[1000]);
 
  // Sort into ascending order:  
  sort(&a[0], &a[1000]);

  for (i = 0; i < 1000; i++) 
    assert(a[i] == i);

  random_shuffle(&a[0], &a[1000]); 

  // Sort into descending order:  
  sort(&a[0], &a[1000], greater<int>()); 

  for (i = 0; i < 1000; i++) 
     assert(a[i] == 1000 - i - 1);
}
