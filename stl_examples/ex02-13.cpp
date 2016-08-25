// Demonstrating generic accumulate algorithm with a reverse 
// iterator.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  cout << "Demonstrating generic accumulate algorithm with "
       << "a reverse iterator." << endl;
  
  float small = (float)1.0/(1 << 26);
  float x[5] = {1.0, 3*small, 2*small, small, small};
  
  // Initialize vector1 to x[0] through x[4]:
  vector<float> vector1(&x[0], &x[5]); 

  cout << "Values to be added: " << endl;

  vector<float>::iterator i;
  cout.precision(10);
  for (i = vector1.begin(); i != vector1.end(); ++i)
    cout << *i << endl;
  cout << endl;

  float sum = accumulate(vector1.begin(), vector1.end(), 
                         (float)0.0);

  cout << "Sum accumulated from left = " <<  sum << endl;

  float sum1 = accumulate(vector1.rbegin(), vector1.rend(),
                          (float)0.0);
  
  cout << "Sum accumulated from right = " << (double)sum1 << endl;
}
