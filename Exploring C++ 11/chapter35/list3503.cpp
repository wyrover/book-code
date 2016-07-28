// Listing 35-3. The point Class Defined with the class Keyword
class point
{
public:
  point() : r_{0.0}, angle_{0.0} {}

  double x() const { return distance() * std::cos(angle()); }
  double y() const { return distance() * std::sin(angle()); }

  double angle()    const { return angle_; }
  double distance() const { return r_; }

  void move_cartesian(double x, double y)
  {
    move_polar(std::sqrt(x*x + y*y), std::atan2(y, x));
  }
  void move_polar(double r, double angle)
  {
    r_ = r;
    angle_ = angle;
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
  double r_;
  double angle_;
};
