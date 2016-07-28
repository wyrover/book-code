// Listing 38-1. Adding a Polymorphic print Function to Every Class Derived from work
class work
{
public:
  work() = default;
  work(work const&) = default;
  work(std::string const& id, std::string const& title) : id_{id}, title_{title} {}
  virtual ~work() {}
  std::string const& id()    const { return id_; }
  std::string const& title() const { return title_; }
  virtual void print(std::ostream&) const {}
private:
  std::string id_;
  std::string title_;
};

class book : public work
{
public:
  book() : work{}, author_{}, pubyear_{0} {}
  book(book const&) = default;
  book(std::string const& id, std::string const& title, std::string const& author,
       int pubyear)
  : work{id, title}, author_{author}, pubyear_{pubyear}
  {}
  std::string const& author() const { return author_; }
  int pubyear()               const { return pubyear_; }
  void print(std::ostream& out)
  const override
  {
    out << author() << ", " << title() << ", " << pubyear() << ".";
  }
private:
  std::string author_;
  int pubyear_; ///< year of publication
};

class periodical : public work
{
public:
  periodical() : work{}, volume_{0}, number_{0}, date_{} {}
  periodical(periodical const&) = default;
  periodical(std::string const& id, std::string const& title, int volume,
             int number,
 std::string const& date)
  : work{id, title}, volume_{volume}, number_{number}, date_{date}
  {}
  int volume()              const { return volume_; }
  int number()              const { return number_; }
  std::string const& date() const { return date_; }
  void print(std::ostream& out)
  const override
  {
    out << title() << ", " << volume() << '(' << number() << "), " << 
           date() << ".";
  }
private:
  int volume_;       ///< volume number
  int number_;       ///< issue number
  std::string date_; ///< publication date
};
