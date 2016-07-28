// Listing 45-8. Throwing an Exception from a noexcept Function
#include <iostream>
#include <exception>

void function() noexcept
{
  throw std::exception{};
}

int main()
{
  try {
    function();
  } catch (std::exception const& ex) {
    std::cout << "Gotcha!\n";
  }
}

