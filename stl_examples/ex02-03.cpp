// Demonstrating generic find algorithm with an array.
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <algo.h>

int main()
{
  cout << "Demonstrating generic find algorithm with "
       << "an array." << endl;
  char* s = "C++ is a better C";
  int len = strlen(s);

  // Search for the first occurrence of the letter e.
  char* where = find(&s[0], &s[len], 'e');

  assert (*where == 'e' && *(where+1) == 't');
}
