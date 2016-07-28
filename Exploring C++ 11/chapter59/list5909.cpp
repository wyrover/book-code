// Listing 59-9. Simple Wrapper for Dynamic Memory
#include <iostream>

class wrapper
{
public:
   wrapper(int x) : p_{new int{x}} {}
   ~wrapper()                      { delete p_; }
   int value() const               { return *p_; }
private:
   int* p_;
};

void print(wrapper w)
{
   std::cout << w.value() << '\n';
}

wrapper increment(wrapper w)
{
   return wrapper(w.value() + 1);
}

int main()
{
  wrapper w{42};
  print(increment(w));
}
