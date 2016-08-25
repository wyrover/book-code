// Using the generic accumulate algorithm to compute a product.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int mult(int x, int y) { return x * y; }

int main()
{
  cout << "Using generic accumulate algorithm to "
       << "compute a product." << endl;

  int x[5] = {2, 3, 5, 7, 11};

  // Initialize vector1 to x[0] through x[4]:
  vector<int> vector1(x, x+5); 
  
  int product = accumulate(vector1.begin(), vector1.end(),
                           1, mult);
    
  assert(product == 2310);
}
