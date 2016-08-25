// Demonstrating generic merge algorithm with an array,
// a list, and a deque.
#include <iostream.h>
#include <string.h>
#include <assert.h>
#include <list.h>
#include <deque.h>
#include <algo.h>

list<char> lst(char* s)
  // Return list<char> containing the characters of s
  // (not including the terminating null).
{
  list<char> x;
  while (*s != '\0')
    x.push_back(*s++);
  return x;
}

deque<char> deq(char* s)
  // Return deque<char> containing the characters of s
  // (not including the terminating null).
{
  deque<char> x;
  while (*s != '\0')
    x.push_back(*s++);
  return x;
}

int main()
{
  cout << "Demonstrating generic merge algorithm with "
       << "an array, a list, and a deque." << endl;
  char* s = "acegikm";
  int len = strlen(s);

  list<char> list1 = lst("bdfhjlnopqrstuvwxyz");

  // Initialize deque1 with 26 copies of the letter x:
  deque<char> deque1(26, 'x');

  // Merge array s and list1, putting result in deque1:
  merge(&s[0], &s[len], list1.begin(), list1.end(),
        deque1.begin());

  assert(deque1 == deq("abcdefghijklmnopqrstuvwxyz"));
}
