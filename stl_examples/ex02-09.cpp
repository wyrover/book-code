// Demonstrating the generic accumulate function.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  cout << "Demonstrating the accumulate function." << endl;
  int x[5] = {2, 3, 5, 7, 11};
  // Initialize vector1 to x[0] through x[4]:
  vector<int> vector1(&x[0], &x[5]); 
  
  int sum = accumulate(vector1.begin(), vector1.end(), 0);
    
  assert(sum == 28);
}
