// Demonstrating multiset construction and insertion.
#include <list.h>
#include <multiset.h>
#include <assert.h>

list<char> lst(char* s)
  // Return list containing the characters of s
  // (not including the terminating null).
{
  list<char> x;
  while (*s != '\0')
    x.push_back(*s++);
  return x;
}

int main()
{
  cout << "Demonstrating multiset construction "
       << "and insertion." << endl;
  list<char> list1 = lst("There is no distinctly native "
                         "American criminal class");
  
  // Put the characters in list1 into multiset1:
  multiset<char, less<char> > multiset1;
  list<char>::iterator i;
  for (i = list1.begin(); i != list1.end(); ++i)
    multiset1.insert(*i);
  
  // Put the characters in multiset1 into list2:
  list<char> list2;
  multiset<char, less<char> >::iterator k;
  for (k = multiset1.begin(); k != multiset1.end(); ++k)
    list2.push_back(*k);

  assert(list2 == lst("       ATaaaaccccdeeeehiiiiiiilll"
                      "mmnnnnnorrrsssstttvy"));
}
