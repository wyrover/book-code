// Listing 66-5. Undefined Behavior Arises from HashableString That Derives from std::string
#include <iostream>
#include <istream>
#include <string>
#include <unordered_set>
#include <utility>

class Hashable
{
public:
   virtual ~Hashable() {}
   virtual unsigned long hash() const = 0;
   virtual bool equalTo(Hashable const&) const = 0;
};

class HashableString : public std::string, public Hashable
{
public:
   HashableString() : std::string{} {}
   HashableString(std::string&& str) : std::string{std::move(str)} {}
   ~HashableString() override {}

   unsigned long hash() const override {
      return std::hash<std::string>()(*this);
   }

   bool equalTo(Hashable const& s) const override {
      return dynamic_cast<HashableString const&>(s) == *this;
   }

};

class string_pool
{
public:
   string_pool() : pool_{} {}
   ~string_pool() {
      while (not pool_.empty()) {
         std::string* ptr{ *pool_.begin() };
         pool_.erase(pool_.begin());
         delete ptr;
      }
   }
   std::string* add(std::string&& str) {
      std::string* ptr = new std::string{std::move(str)};
      pool_.insert(ptr);
      return ptr;
   }
private:
   std::unordered_set<std::string*> pool_;
};

int main()
{
   string_pool pool{};
   HashableString str{};
   while (std::cin >> str)
   {
      std::cout << "hash of \"" << str << "\" = " << str.hash() << '\n';
      pool.add(std::move(str));
   }
}

