// Listing 68-2. Qualified Name Lookup
#include <chrono>
#include <iostream>

namespace outer {
   namespace inner {
      class base {
      public:
         int value() const { return 1; }
         static int value(long x) { return static_cast<int>(x); }
      };
   }

   template<class T>
   class derived : public inner::base {
   public:
      typedef T value_type;
      using inner::base::value;
      static value_type example;
      value_type value(value_type i) const { return i * example; }
   };

   template<class T>
   typename derived<T>::value_type derived<T>::example = 2;
}

template<class T>
class more_derived : public outer::derived<T>{
public:
   typedef outer::derived<T> base_type;
   typedef typename base_type::value_type value_type;
   more_derived(value_type v) : value_{this->value(v)} {}
   value_type get_value() const { return value_; }
private:
   value_type value_;
};
  
int main()
{
   std::chrono::system_clock::time_point now{std::chrono::system_clock::now()};
   std::cout << now.time_since_epoch().count() << '\n';

   outer::derived<int> d;
   std::cout << d.value() << '\n';
   std::cout << d.value(42L) << '\n';
   std::cout << outer::inner::base::value(2) << '\n';

   more_derived<int> md(2);
   std::cout << md.get_value() << '\n';
}
