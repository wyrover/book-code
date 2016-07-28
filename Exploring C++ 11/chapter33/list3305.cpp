// Listing 33-5. Constructors for Class point
struct point
{
  point()
  : point{0.0, 0.0}
  {}
  point(double x, double y)
  : x_{x}, y_{y}
  {}
  point(point const& pt)
  : point{pt.x_, pt.y_}
  {}
  double x_;
  double y_;
};
