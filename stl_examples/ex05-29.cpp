// Illustrating the generic lexicographical_compare algorithm.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  bool result;

  // Initialize vectors of characters:
  vector<char> vecChar1(5);
  char elem1[5] = {'h', 'e', 'l', 'i', 'o'};
  for(int i = 0; i<5; i++)
    vecChar1[i] = elem1[i];

  vector<char> vecChar2(5);
  char elem2[5] = {'h', 'e', 'l', 'l', 'o'};

  for(i = 0; i<5; i++)
    vecChar2[i] = elem2[i];

  // Show that vecChar1 is lexicographically less than 
  // vecChar2:
  result = lexicographical_compare(vecChar1.begin(),
      vecChar1.end(), vecChar2.begin(), vecChar2.end());

  assert(result == true);
}
