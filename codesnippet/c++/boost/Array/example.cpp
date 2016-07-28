#include <iostream>
#include <boost/array.hpp>

using namespace std;

int main(int argc, char *argv[])
{
  
  // An array can be initialized with a brace-enclosing, 
  // comma-separated list of initializers for the elements of the container, 
  // written in increasing subscript order:
  // constructor
  // --------------------------------------------------------------
  boost::array<int,4> a = { { 1, 2, 3, 4} };
 
  // constructor operator = 
  boost::array<int,4> b = a;

 
  // capacity 
  //-----------------------------------------------------------------
  // empty() 
  if (b.empty()) {
    cout << "array b is empty" << endl;
  } else {
    cout << "array b is not empty" << endl;
  }
  // size()
  cout << "size of array a is : " << a.size() << endl;
  // max_size()
  cout << "max size of array a is : " << a.size() << endl;

   
  // comparison
  if (a == b) {
    cout << "the array b is the same as the array a" << endl;
  }

  boost::array<int, 4> c = { { 2, 3, 4, 5}}; 
  if (a < c) {
    cout << "the array a is less then array c" << endl;
  }
 
  if (a != c) {
    cout << "the array a is not the same as the array c" << endl;
  } 

  // element access
  // [], at, front, back
  // --------------------------------------------------------------
  cout << "The first element : " << a.front() << endl;
  cout << "The last element : " << a.back() << endl;
  cout << "The second element : " << a[1] << endl; 
  cout << "The third element : " << a.at(2) << endl;
  
  int *data = a.data();

  for (int i = 0; i < 4; ++i) {
    cout << "a[" << i << "] = " << data[i] << endl;
  }

  int *elems = a.c_array();

  for (int i = 0; i < 4; ++i) {
    cout << "a[" << i << "] = " << elems[i] << endl;
  }

  //  iterator 
  //  -----------------------------------------------------------
 
  boost::array<int, 4>::iterator iter = a.begin(); 
  
  for (; iter != a.end(); ++iter) {
    cout << *iter << "\t";
    *iter = (*iter) * 2;
  }
  cout << endl; 

  boost::array<int, 4>::const_iterator citer =a.begin(); 
  for (; citer != a.end(); ++citer) {
    cout << *citer << "\t"; 
  }

  // algorithm 
  swap(a, b);
  for (int i = 0; i < 4; ++i) {
    cout << "a[" << i << "] = " << a[i] << endl;
    cout << "b[" << i << "] = " << b[i] << endl;
  }
 
  return 0;
}
