#ifndef STRING_POOL_HPP_
#define STRING_POOL_HPP_

#include <string>
#include <unordered_set>

/// Pool of strings.
/// From Listing 66-5.
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

#endif

