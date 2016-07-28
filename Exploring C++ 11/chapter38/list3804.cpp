// Listing 38-4. Adding a Class movie
class movie : public work
{
public:
  movie() : work{}, runtime_{0} {}
  movie(movie const&) = default;
  movie(std::string const& id, std::string const& title, int runtime)
  : work{id, title}, runtime_{runtime}
  {}
  int runtime() const { return runtime_; }
private:
  int runtime_; ///< running length in minutes
};
