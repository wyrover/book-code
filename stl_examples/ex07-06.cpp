// Computing an inner product of tuples represented as maps.
#include <map.h>
#include <iostream.h>

int main()
{
  cout << "Computing an inner product of tuples "
       << "represented as maps." << endl;

  const long N = 600000; // Length of tuples x and y
  const long S = 10; // Sparseness factor

  cout << "\nInitializing..." << flush;
  map<long, double, less<long> > x, y;
  for (long k = 0; 3 * k * S < N; ++k) 
    x[3 * k * S] = 1.0;
  for (k = 0; 5 * k * S < N; ++k) 
    y[5 * k * S] = 1.0;

  cout << "\n\nComputing inner product taking advantage "
       << "of sparseness: " << flush;

  double sum;
  map<long, double, less<long> >::iterator ix, iy;
  for (sum = 0.0, ix = x.begin(); ix != x.end(); ++ix) {
    long i = (*ix).first;
    iy = y.find(i);
    if (iy != y.end())
      sum += (*ix).second * (*iy).second;
  }
  cout << sum << endl;
}
