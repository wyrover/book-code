// Illustrating the use of an adaptor for pointers to functions.
#include <set.h>
#include <bstring.h>

bool less1(const string& x, const string& y)
{
  return x < y;
}

bool greater1(const string& x, const string& y)
{
  return x > y;
}

int main()
{
  typedef 
    set<string, 
        pointer_to_binary_function<const string&, 
                                   const string&, bool> >  
    set_type1;

  set_type1 set1(ptr_fun(less1));

  set1.insert("the");
  set1.insert("quick");
  set1.insert("brown");
  set1.insert("fox");

  set_type1::iterator i;
  for (i = set1.begin(); i != set1.end(); ++i) 
    cout << *i << " ";
  cout << endl;
  
  set_type1 set2(ptr_fun(greater1));

  set2.insert("the");
  set2.insert("quick");
  set2.insert("brown");
  set2.insert("fox");

  for (i = set2.begin(); i != set2.end(); ++i) 
    cout << *i << " ";
  cout << endl;
}  

