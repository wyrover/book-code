// Listing 48-1. Writing an Absolute Value Function
T absval(T x)
{
  if (x < 0)
    return -x;
  else
    return x;
}
