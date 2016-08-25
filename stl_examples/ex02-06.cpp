// Demonstrating generic find algorithm with a deque.
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <deque.h>
#include <algo.h>

int main()
{
  cout << "Demonstrating generic find algorithm with "
       << "a deque." << endl;
  char* s = "C++ is a better C";
  int len = strlen(s);

  // Initialize deque1 with the contents of string s.
  deque<char> deque1(&s[0], &s[len]);

  // Search for the first occurrence of the letter e.
  deque<char>::iterator
    where = find(deque1.begin(), deque1.end(), 'e');
  assert (*where == 'e' && *(where+1) == 't');
}
