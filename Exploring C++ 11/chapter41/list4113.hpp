// Listing 41-13. The Original point.hpp File
#ifndef POINT_HPP_
#define POINT_HPP_
class point
{
public:
  point() : point{0, 0} {}
  point(int x, int y) : x_{x}, y_{y} {}
  int x() const { return x_; }
  int y() const { return y_; }
private:
  int y_, x_;
};
#endif // POINT_HPP_
