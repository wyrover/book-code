// Computing an inner product of tuples represented as vectors.
#include <vector.h>
#include <iostream.h>

int main()
{
  cout << "Computing an inner product of tuples "
       << "represented as vectors." << endl;

  const long N = 600000; // Length of tuples x and y
  const long S = 10; // Sparseness factor

  cout << "\nInitializing..." << flush;
  vector<double> x(N, 0.0), y(N, 0.0);

  for (long k = 0; 3 * k * S < N; ++k) 
    x[3 * k * S] = 1.0;
  for (k = 0; 5 * k * S < N; ++k) 
    y[5 * k * S] = 1.0;

  cout << "\n\nComputing inner product by brute force: " 
       << flush;
  double sum;
  for (sum = 0.0, k = 0; k < N; ++k)
    sum += x[k] * y[k];
    
  cout << sum << endl;
}
