// Illustrating the generic replace algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>
#include <bstring.h>

int main()
{
  vector<char> vecChar = string("FERRARI");

  // Replace all occurrences of R by S:
  replace(vecChar.begin(), vecChar.end(), 'R', 'S');

  assert(string(vecChar) == string("FESSASI"));
}
