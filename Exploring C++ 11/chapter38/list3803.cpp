// Listing 38-3. Output Operator for Class work
std::ostream& operator<<(std::ostream& out, work const& w)
{
  w.print(out);
  return out;
}
