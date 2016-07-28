// Listing 56-6. Parsing Comments in the Miles-per-Gallon Data File
#include <iostream>
#include <sstream>
#include <string>

int main()
{
   double total_fuel{0.0};
   double total_distance{0.0};
   double prev_odometer{0.0};
   std::string line{};
   int linenum{0};
   bool error{false};
   while (std::getline(std::cin, line))
   {
      ++linenum;
      std::istringstream input{line};
      char comment{};
      if (input >> comment and comment != '#')
      {
         input.unget();
         double odometer{};
         if (input >> odometer)
         {
           double fuel{};
            if (not (input >> fuel))
            {
               std::cerr << "Missing fuel consumption on line " << linenum << '\n';
               error = true;
            }
            else if (fuel != 0)
            {
               double distance{odometer - prev_odometer};
               std::cout << distance / fuel << '\n';
               total_fuel += fuel;
               total_distance += distance;
               prev_odometer = odometer;
            }
         }
      }
   }
   if (total_fuel != 0)
   {
      std::cout << "Net MPG=" << total_distance / total_fuel;
      if (error)
         std::cout << " (estimated, due to input error)";
      std::cout << '\n';
   }
}
