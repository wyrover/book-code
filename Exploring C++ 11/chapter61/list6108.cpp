// Listing 61-8. Mystery Program That Uses unique_ptr
#include <iostream>
#include <memory>

class mystery
{
public:
  mystery() {}
  mystery(mystery const&) { throw "oops"; }
};

class demo
{
public:
  demo(int* x, mystery m, int* y) : x_{x}, m_{m}, y_{y} {}
  demo(demo const&) = delete;
  demo& operator=(demo const&) = delete;
  int x() const { return *x_; }
  int y() const { return *y_; }
private:
  std::unique_ptr<int> x_;
  mystery            m_;
  std::unique_ptr<int> y_;
};

int main()
{
  demo d{new int{42}, mystery{}, new int{24}};
  std::cout << d.x() << d.y() << '\n';
}
