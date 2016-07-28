// Listing 5-3. Getting the User’s Name and Age
#include <iostream>
#include <string>

int main()
{
   std::cout << "What is your name? ";
   std::string name{};
   std::cin >> name;

   std::cout << "Hello, " << name << ", how old are you? ";
   int age{};
   std::cin >> age;

   std::cout << "Good-bye, " << name << ". You are " << age << " year";
   if (age != 1)
      std::cout << 's';
   std::cout << " old.\n";
}
