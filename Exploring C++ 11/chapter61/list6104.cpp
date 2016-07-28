// Listing 61-4. Using the unique_ptr Class Template
#include <iostream>
#include <memory>

class see_me
{
public:
  see_me(int x) : x_{x} { std::cout <<  "see_me(" << x_ << ")\n"; }
  ~see_me()             { std::cout << "~see_me(" << x_ << ")\n"; }
  int value() const     { return x_; }
private:
  int x_;
};

std::unique_ptr<see_me> nothing(std::unique_ptr<see_me>&& arg)
{
  return std::move(arg);
}

template<class T>
std::unique_ptr<T> make(int x)
{
  return std::unique_ptr<T>{new T{x}};
}

int main()
{
  std::cout << "program begin...\n";
  std::unique_ptr<see_me> sm{make<see_me>(42)};
  std::unique_ptr<see_me> other;
  other = nothing(std::move(sm));
  if (sm.get() == nullptr)
    std::cout << "sm is null, having lost ownership of its pointer\n";
  if (other.get() != nullptr)
    std::cout << "other now has ownership of the int, " <<
                 other->value() << '\n';
  std::cout << "program ends...\n";
}
