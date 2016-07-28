// Listing 56-3. Computing Miles per Gallon
#include <iostream>

int main()
{
   double total_fuel{0.0};
   double total_distance{0.0};
   double prev_odometer{0.0};
   double fuel{}, odometer{};
   while (std::cin >> odometer >> fuel)
   {
      if (fuel != 0)
      {
         double distance{odometer - prev_odometer};
         std::cout << distance / fuel << '\n';
         total_fuel += fuel;
         total_distance += distance;
         prev_odometer = odometer;
      }
   }
   if (total_fuel != 0)
      std::cout << "Net MPG=" << total_distance / total_fuel << '\n';
}
