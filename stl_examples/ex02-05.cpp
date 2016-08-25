// Demonstrating generic find algorithm with a list.
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <list.h>
#include <algo.h>

int main()
{
  cout << "Demonstrating generic find algorithm with "
       << "a list." << endl;
  char* s = "C++ is a better C";
  int len = strlen(s);

  // Initialize list1 with the contents of string s.
  list<char> list1(&s[0], &s[len]);

  // Search for the first occurrence of the letter e.
  list<char>::iterator
    where = find(list1.begin(), list1.end(), 'e');
  assert (*where == 'e' && *(++where) == 't');
}
