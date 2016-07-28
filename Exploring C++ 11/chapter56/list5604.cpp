// Listing 56-4. Computing Liters per Kilometer
#include <iostream>

int main()
{
   double total_fuel{0.0};
   double total_distance{0.0};
   double prev_odometer{0.0};
   double fuel{}, odometer{};
   while (std::cin >> odometer >> fuel)
   {
      double distance{odometer - prev_odometer};
      if (distance != 0)
      {
         std::cout << fuel / distance << '\n';
         total_fuel += fuel;
         total_distance += distance;
         prev_odometer = odometer;
      }
   }
   if (total_distance != 0)
      std::cout << "Net LPK=" << total_fuel / total_distance << '\n';
}
