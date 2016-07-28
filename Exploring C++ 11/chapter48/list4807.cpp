// Listing 48-7. The std::min Algorithm
template<class T>
T min(T a, T b)
{
  if (a < b)
    return a;
  else if (b < a)
    return b;
  else
    return a; // a and b are equivalent, so return a
}
