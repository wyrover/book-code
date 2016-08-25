// Demonstrating generic find algorithm with a vector.
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <vector.h>
#include <algo.h>

int main()
{
  cout << "Demonstrating generic find algorithm with "
       << "a vector." << endl;
  char* s = "C++ is a better C";
  int len = strlen(s);

  // Initialize vector1 with the contents of string s.
  vector<char> vector1(&s[0], &s[len]);

  // Search for the first occurrence of the letter e.
  vector<char>::iterator
    where = find(vector1.begin(), vector1.end(), 'e');
  assert (*where == 'e' && *(where + 1) == 't');
}
