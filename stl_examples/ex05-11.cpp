// Illustrating the generic fill and fill_n algorithms.
#include <algo.h>
#include <vector.h>
#include <bstring.h>
#include <assert.h>

int main()
{
  vector<char> vector1 = string("Hello there");

  // Fill first 5 positions of vector1 with X's:
  fill(vector1.begin(), vector1.begin() + 5, 'X');

  assert(string(vector1) == string("XXXXX there"));

  // Fill 3 more positions with Y's.
  fill_n(vector1.begin() + 5, 3, 'Y');

  assert(string(vector1) == string("XXXXXYYYere"));
}
