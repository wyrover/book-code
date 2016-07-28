// Listing 49-2. The move_by Member Function
template<class T>
void point<T>::move_by(T x, T y)
{
  x_ += x;
  y_ += y;
}
