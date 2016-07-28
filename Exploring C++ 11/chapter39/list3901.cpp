// Listing 39-1. Computing BMI
#include <iostream>

typedef int height;
typedef int weight;
typedef int bmi;

bmi compute_bmi(height h, weight w)
{
  return w * 10000 / (h * h);
}

int main()
{
  std::cout << "Height in centimeters: ";
  height h{};
  std::cin >> h;

  std::cout << "Weight in kilograms: ";
  weight w{};
  std::cin >> w;

  std::cout << "Body-mass index = " << compute_bmi(w, h) << '\n';
}
