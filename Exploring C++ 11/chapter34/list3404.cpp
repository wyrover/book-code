// Listing 34-4. Calling const and Non-const Member Functions
#include <cmath>
#include <iostream>

// Use the same point definition as Listing 34-3
struct point
{
  /// Distance to the origin.
  double distance()
  const
  {
    return std::sqrt(x*x + y*y);
  }
  /// Angle relative to x-axis.
  double angle()
  const
  {
    return std::atan2(y, x);
  }

  /// Add an offset to x and y.
  void offset(double off)
  {
    offset(off, off);
  }
  /// Add an offset to x and an offset to y
  void offset(double  xoff, double yoff)
  {
    x = x + xoff;
    y = y + yoff;
  }

  /// Scale x and y.
  void scale(double mult)
  {
    this->scale(mult, mult);
  }
  /// Scale x and y.
  void scale(double xmult, double ymult)
  {
    this->x = this->x * xmult;
    this->y = this->y * ymult;
  }
  double x;
  double y;
};

void print_polar(point const& pt)
{
  std::cout << "{ r=" << pt.distance() << ", angle=" << pt.angle() << " }\n";
}

void print_cartesian(point const& pt)
{
  std::cout << "{ x=" << pt.x << ", y=" << pt.y << " }\n";
}

int main()
{
  point p1{}, p2{};
  double const pi{3.141592653589792};
  p1.x = std::cos(pi / 3);
  p1.y = std::sin(pi / 3);
  print_polar(p1);
  print_cartesian(p1);
  p2 = p1;
  p2.scale(4.0);
  print_polar(p2);
  print_cartesian(p2);
  p2.offset(0.0, -2.0);
  print_polar(p2);
  print_cartesian(p2);
}
