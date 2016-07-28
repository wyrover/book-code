// Listing 49-1. The point Class Template
template<class T>
class point {
public:
   point(T x, T y) : x_{x}, y_{y} {}
   T x() const { return x_; }
   T y() const { return y_; }
   void move_to(T x, T y);           ///< Move to absolute coordinates (x, y)
   void move_by(T x, T y);           ///< Add (x, y) to current position
private:
   T x_, y_;
};

template<class T>
void point<T>::move_to(T x, T y)
{
  x_ = x;
  y_ = y;
}
