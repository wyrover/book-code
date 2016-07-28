// Listing 50-1. The point Class Template
template<class T>
class point
{
public:
  typedef T value_type;
  point(T const& x, T const& y) : x_{x}, y_{y} {}
  point() : point{T{}, T{}} {}
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
