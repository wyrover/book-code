// Illustrating the generic copy and copy_backward algorithms.
#include <vector.h>
#include <algo.h>
#include <iostream.h>
#include <bstring.h>
#include <assert.h>
int main()
{
  vector<char> vector1 = 
    string("abcdefghihklmnopqrstuvwxyz");

  vector<char> vector2(vector1.size());

  // Copy vector1 to vector2:
  copy(vector1.begin(), vector1.end(), 
       vector2.begin());

  assert(vector1 == vector2);

  // Shift the contents of vector1 left by 4 positions:
  copy(vector1.begin() + 4, vector1.end(), 
       vector1.begin());

  assert(string(vector1) == 
         string("efghihklmnopqrstuvwxyzwxyz"));

  // Shift it right by 2 positions:
  copy_backward(vector1.begin(), vector1.end() - 2, 
                vector1.end());

  assert(string(vector1) == 
         string("efefghihklmnopqrstuvwxyzwx"));
}
