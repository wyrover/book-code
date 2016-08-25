// Illustrating the generic accumulate algorithm.
#include <algo.h>
#include <function.h>
#include <assert.h>

int main()
{
  int result;
  int x[20];
  
  for (int i = 0; i < 20; i++)
    x[i] = i;

  // Show that 5 + 0 + 1 + 2 + ... + 19 == 195:
  result = accumulate(&x[0], &x[20], 5);
  assert(result == 195);

  // Show that 10 * 1 * 2 * 3 * 4 == 240:
  result = accumulate(&x[1], &x[5], 10, times<int>());
  assert(result == 240);
}
