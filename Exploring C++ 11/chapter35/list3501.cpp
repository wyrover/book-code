// Listing 35-1. The point Class with Access-Level Specifiers
struct point
{
public:
  point() : point{0.0, 0.0} {}
  point(double x, double y) : x_{x}, y_{y} {}
  point(point const&) = default;

  double x() const { return x_; }
  double y() const { return y_; }

  double angle()    const { return std::atan2(y(), x()); }
  double distance() const { return std::sqrt(x()*x() + y()*y()); }

  void move_cartesian(double x, double y)
  {
    x_ = x;
    y_ = y;
  }
  void move_polar(double r, double angle)
  {
    move_cartesian(r * std::cos(angle), r * std::sin(angle));
  }

  void scale_cartesian(double s)       { scale_cartesian(s, s); }
  void scale_cartesian(double xs, double ys)
  {
    move_cartesian(x() * xs, y() * ys);
  }
  void scale_polar(double r)           { move_polar(distance() * r, angle()); }
  void rotate(double a)                { move_polar(distance(), angle() + a); }
  void offset(double o)                { offset(o, o); }
  void offset(double xo, double yo)    { move_cartesian(x() + xo, y() + yo); }

private:
  double x_;
  double y_;
};
