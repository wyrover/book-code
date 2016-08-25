// Illustrating the generic rotate algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>
#include <bstring.h>

int main()
{
  vector<char> vecChar = string("Software Engineering ");

  // Rotate the vector so that "Engineering " comes first:
  rotate(vecChar.begin(), vecChar.begin() + 9,
         vecChar.end());

  assert(string(vecChar) == 
         string("Engineering Software "));
}
