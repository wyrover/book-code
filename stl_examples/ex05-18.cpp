// Illustrating the generic swap algorithm.
#include <algobase.h>
#include <assert.h>

int main()
{
  int high = 250, low = 0;

  swap(high, low);

  assert(high == 0 && low  == 250);
}
