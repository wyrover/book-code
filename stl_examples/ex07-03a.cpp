// Demonstrating multiset erase functions.
// This is the modification of Example 7-3 discussed on p. 160.
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

template <class Container>
list<char> lst1(const Container& m)
  // Return list containing the characters in m
{
  list<char> x;
  copy(m.begin(), m.end(), back_inserter(x));
  return x;
}

int main()
{
  cout << "Demonstrating multiset erase functions" << endl;
  list<char> list1 = lst("There is no distinctly native "
                         "American criminal class");

  // Put the characters in list1 into multiset1:
  multiset<char, less<char> > multiset1;
  copy(list1.begin(), list1.end(),
       inserter(multiset1, multiset1.end()));

  assert(lst1(multiset1) == 
         lst("       ATaaaaccccdeeeehiiiiiiilll"
             "mmnnnnnorrrsssstttvy"));

  multiset1.erase('a');
  assert(lst1(multiset1) == 
         lst("       ATccccdeeeehiiiiiiilll"
             "mmnnnnnorrrsssstttvy"));

  multiset<char, less<char> >::iterator i = 
    multiset1.find('e');
  
  multiset1.erase(i);
  assert(lst1(multiset1) == 
         lst("       ATccccdeeehiiiiiiilll"
             "mmnnnnnorrrsssstttvy"));

  i = multiset1.find('T');
  multiset<char, less<char> >::iterator j = 
    multiset1.find('v');
  
  multiset1.erase(i, j);
  assert(lst1(multiset1) == lst("       Avy"));
}
