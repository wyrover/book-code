// Listing 56-8. Adding Error-Checking for Each Line of Input
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
      std::string::size_type comment{line.find('#')};
      if (comment != std::string::npos)
         line.erase(comment);
      std::istringstream input{line};
      double odometer{};
      if (input >> odometer)
      {
         double fuel{};
         char check{};
         if (not (input >> fuel))
         {
            std::cerr << "Missing fuel consumption on line " << linenum << '\n';
            error = true;
         }
         else if (input >> check)
         {
            std::cerr << "Extra text on line " << linenum << '\n';
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
   if (total_fuel != 0)
   {
      std::cout << "Net MPG=" << total_distance / total_fuel;
      if (error)
         std::cout << " (estimated, due to input error)";
      std::cout << '\n';
   }
}
