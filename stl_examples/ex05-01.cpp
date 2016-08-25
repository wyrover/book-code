// Using an in-place generic sort algorithm.
#include <algo.h>
#include <assert.h>

int main() {
  int a[1000];
  for (int i = 0; i < 1000; i++) 
    a[i] = 1000 - i - 1;

  sort(&a[0], &a[1000]);

  for (i = 0; i < 1000; i++) 
    assert(a[i] == i);
}
