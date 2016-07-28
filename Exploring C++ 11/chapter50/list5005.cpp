// Listing 50-5. The Primary std::less Class Template
template<class T>
struct less
{
   bool operator()(T const& a, T const& b) const { return a < b; }
};
