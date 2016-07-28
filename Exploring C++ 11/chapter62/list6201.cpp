// Listing 62-1. Array Type in Function Parameters
#include <iostream>

int sum(int* array, int size);

int main()
{
  int data[5]{ 1, 2, 3, 4, 5 };
  std::cout << sum(data, 5) << '\n';
}

int sum(int array[], int size)
{
  int result{0};
  while (size-- != 0)
    result += array[size];
  return result;
}
