// Listing 66-9. Using mix-in Class Template
#include <string>
#include <utility>
#include <iostream>
#include "assignment_mixin.hpp" // Listing 66-8

class thing: public assignment_mixin<thing> {
public:
   thing() : value_{} {}
   thing(std::string&& s) : value_{std::move(s)} {}
   void swap(thing& other) { value_.swap(other.value_); }

   std::string const& str() const { return value_; }

private:
   std::string value_;
};

int main()
{
   thing one{};
   thing two{"two"};
   one = two;

   std::cout << "one=" << one.str() << "\ntwo=" << two.str() << '\n';
}

