// Illustrating the assert macro.
// This is the INCORRECT modification of Example 1-1
// discussd on p. 17.
#include <iostream.h>
#include <assert.h>
#include <string.h>

int main()
{
  cout << "Illustrating the assert macro." << endl;

  char* string0 = "mark twain";
  char string1[20];
  char string2[20];
  strcpy(string1, string0);
  strcpy(string2, string0);
  int N1 = strlen(string1);
  int N2 = strlen(string2);

  assert(N1 == N2);

  // Put the reverse of string1 in string2:
  for (int k = 0; k != N1; ++k)
    string2[k] = string1[N1-k];
              // INCORRECT, should be string1[N1-1-k]

  assert(strcmp(string2, "niawt kram") == 0);
  // This assertion fails because of the error.
}
