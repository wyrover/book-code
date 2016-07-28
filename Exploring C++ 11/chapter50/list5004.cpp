// Listing 50-4. Specializing std::less to Compare person Objects by Name
#include <functional>
#include <iostream>
#include <map>
#include <string>

class person {
public:
   person() : name_{}, address_{}, phone_{} {}
   person(std::string const& name,
          std::string const& address,
          std::string const& phone)
   : name_{name}, address_{address}, phone_{phone}
   {}
   std::string const& name()    const { return name_; }
   std::string const& address() const { return address_; }
   std::string const& phone()   const { return phone_; }
private:
   std::string name_, address_, phone_;
};

namespace std {
   template<>
   struct less<person> {
      bool operator()(person const& a, person const& b) const {
         return a.name() < b.name();
      }
   };
}

int main()
{
   std::map<person, int> people;
   people[person{"Ray", "123 Erewhon", "555-5555"}] = 42;
   people[person{"Arthur", "456 Utopia", "123-4567"}]= 10;
   std::cout << people.begin()->first.name() << '\n';
}
