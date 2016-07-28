// Listing 63-1. Working with shared_ptr
#include <iostream>
#include <memory>
#include <vector>

class see_me
{
public:
  see_me(int x) : x_{x} { std::cout <<  "see_me(" << x_ << ")\n"; }
  ~see_me()             { std::cout << "~see_me(" << x_ << ")\n"; }
  int value() const     { return x_; }
private:
  int x_;
};

std::shared_ptr<see_me> does_this_work(std::shared_ptr<see_me> x)
{
  std::shared_ptr<see_me> y{x};
  return y;
}

int main()
{
  std::shared_ptr<see_me> a{}, b{};
  a = std::make_shared<see_me>(42);
  b = does_this_work(a);
  std::vector<std::shared_ptr<see_me>> v{};
  v.push_back(a);
  v.push_back(b);
}
