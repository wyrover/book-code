// Demonstrating the generic find algorithm with input iterators 
// associated with arrays, lists, and iostreams.
#include <iostream.h>
#include <assert.h>
#include <algo.h>
#include <list.h>

int main() 
{
  // Initialize array a with 10 integers:
  int a[10] = {12, 3, 25, 7, 11, 213, 7, 123, 29, -31};
    
  // Find the first element equal to 7 in the array:
  int* ptr = find(&a[0], &a[10], 7);
    
  assert(*ptr == 7 && *(ptr+1) == 11);

  // Initialize list1 with the same integers as in array a:
  list<int> list1(&a[0], &a[10]);

  // Find the first element equal to 7 in list1:
  list<int>::iterator i = find(list1.begin(),
                               list1.end(),7);
    
  assert(*i == 7 && *(++i) == 11);

  cout << "Type some characters, including an 'x' followed\n"
    << "by at least one nonwhite-space character: " << flush;

  istream_iterator<char, ptrdiff_t> in(cin);
  istream_iterator<char, ptrdiff_t> eos;
  find(in, eos, 'x');

  cout << "The first nonwhite-space character following\n"
       << "the first 'x' was '" << *(++in) << "'." << endl;
}
