// Listing 38-6. Adding a print Member Function to the movie Class
class movie : public work
{
public:
  movie() : work{}, runtime_{0} {}
  movie(movie const&) = default;
  movie(std::string const& id, std::string const& title, int runtime)
  : work{id, title}, runtime_{runtime}
  {}
  int runtime() const { return runtime_; }
  void print(std::ostream& out)
  const override
  {
    out << title() << " (" << runtime() << " min)";
  }
private:
  int runtime_; ///< running length in minutes
};
