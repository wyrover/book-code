// Illustrating the random_shuffle algorithm.
#include <algo.h>
#include <vector.h>
#include <function.h>  
#include <iostream.h>

int main() {
  const int N = 20;
  vector<int> vector1(N);
  for (int i = 0; i < N; ++i) 
    vector1[i] = i;

  for (int j = 0; j < 3; ++j) {
    // Randomly shuffle the integers in vector1:
    random_shuffle(vector1.begin(), vector1.end()); 

    // Output the contents of vector1:
    copy(vector1.begin(), vector1.end(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
  }
}
