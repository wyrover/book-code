// Demonstrating generic merge algorithm, merging parts of an
// array and a deque, putting the result into a list.
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
  cout << "Demonstrating generic merge algorithm,\n"
       << "merging parts of an array and a deque, putting\n"
       << "the result into a list." << endl;
  char* s = "acegikm";
  
  deque<char> deque1 = deq("bdfhjlnopqrstuvwxyz");

  // Initialize list1 with 26 copies of the letter x:
  list<char> list1(26, 'x');

  // Merge first 5 letters in array s with first 10 in
  // deque1, putting result in list1:
  merge(&s[0], &s[5], deque1.begin(), deque1.begin() + 10,
        list1.begin());

  assert(list1 == lst("abcdefghijlnopqxxxxxxxxxxx"));
}
