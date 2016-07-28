// Listing 50-3. Specializing point<double> and Testing the Code
#include <iostream>

template<class T>
class point
{
public:
  typedef T value_type;
  point(T const& x, T const& y) : x_{x}, y_{y} {}
  point() : point{T{}, T{}} { std::cout << "point<>()\n"; }
  T const& x() const { return x_; }
  T const& y() const { return y_; }
  void move_absolute(T const& x, T const& y) {
    x_ = x;
    y_ = y;
  }
  void move_relative(T const& dx, T const& dy) {
    x_ += dx;
    y_ += dy;
  }
private:
  T x_;
  T y_;
};

template<>
class point<double>
{
public:
  typedef double value_type;
  point(double  x, double y) : x_(x), y_(y) {}
  point() : point{0.0, 0.0} { std::cout << "point<double> specialization\n"; }
  double x() const { return x_; }
  double y() const { return y_; }
  void move_absolute(double x, double y) {
    x_ = x;
    y_ = y;
  }
  void move_relative(double dx, double dy) {
    x_ += dx;
    y_ += dy;
  }
private:
  double x_;
  double y_;
};

int main()
{
  point<short> s;
  point<double> d;
  s.move_absolute(10, 20);
  d.move_absolute(1.23, 4.56);
}
