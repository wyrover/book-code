// Demonstrating multiset search member functions.
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
  cout << "Demonstrating multiset search member functions." 
       << endl;
  list<char> list1 = lst("There is no distinctly native "
                         "American criminal class"),
             list2 = lst("except Congress. - Mark Twain");

  // Put the characters in list1 into multiset1:
  multiset<char, less<char> > multiset1;
  copy(list1.begin(), list1.end(),
       inserter(multiset1, multiset1.end()));

  assert(lst1(multiset1) == 
         lst("       ATaaaaccccdeeeehiiiiiiilll"
             "mmnnnnnorrrsssstttvy"));

  multiset<char, less<char> >::iterator i = 
    multiset1.lower_bound('c');
  multiset<char, less<char> >::iterator j = 
    multiset1.upper_bound('r');
  
  multiset1.erase(i, j);

  assert(lst1(multiset1) == lst("       ATaaaasssstttvy"));
  
  list<char> found, not_found;
  list<char>::iterator k;
  for (k = list2.begin(); k != list2.end(); ++k)
    if (multiset1.find(*k) != multiset1.end())
      found.push_back(*k);
    else
      not_found.push_back(*k);

  assert(found == lst("t ss  a Ta"));
  assert(not_found == lst("excepCongre.-Mrkwin"));
}
